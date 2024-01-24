
#include "stepper.h"

Stepper::Stepper(int pin_dir = 2, int pin_step = 3, int pin_nEnable = 4, int pin_M0 = 5, int pin_M1 = 6, int pin_M2 = 7){
    step_pin = pin_step;
    direction_pin = pin_dir;
    nEnable_pin = pin_nEnable;
    M0 = pin_M0;
    M1 = pin_M1;
    M2 = pin_M2;
    _speed = 200;
    _accel = 200;

    pinMode(step_pin,OUTPUT);
    digitalWrite(step_pin,LOW);
    pinMode(direction_pin,OUTPUT);
    digitalWrite(direction_pin,LOW);

    pinMode(endstop1_pin, INPUT_PULLUP);
    pinMode(endstop2_pin, INPUT_PULLUP);
}

void Stepper::change_microstep_resolution(short int resolution)
{
    _microstep_resolution = microstep_table[resolution][3];
    digitalWrite(M0, microstep_table[resolution][0]);
    digitalWrite(M1, microstep_table[resolution][1]);
    digitalWrite(M2, microstep_table[resolution][2]);
}

void Stepper::change_profile(int speed, int accel)
{
    _speed = speed;
    _accel = accel;
}

void Stepper::calibration_direction(int endstop_offset, int direction, bool home, double max_calibration_travel)
{   
    move_relative(direction*endstop_position); //move towards the endstop
    if(!home){
      endstop_position = _current_position;
      move_relative(-direction*endstop_position);
    }
    if(home){
      move_relative(-direction*endstop_position);
      _current_position = 0;
    }
    move_relative(-direction*endstop_offset);
}


void Stepper::calibration(unsigned int endstop_offset = 0)
{   

    double _speed_copy = _speed;
    _speed = _speed_calibration;

    calibration_direction(endstop_offset, -1, true, 1E6);

    calibration_direction(endstop_offset, 1, false, 1E6);

    _speed = _speed_copy;
}


void Stepper::setup_move(double absolute_pos)
{
    absolute_position = absolute_pos;
    
    //Test if target position is in calibrated range
    if(absolute_position < 0 || absolute_position > (_current_position+endstop_position)){
      absolute_position = _current_position;
      out_ofRange();
    }
    
    this_move_period = 1000000.0/sqrt(2.0*_accel); // first period in US
    running_period_US = 1000000.0/_speed;
    deceleration_distance = (_speed*_speed)/(2.0*_accel);

    relative_distance = absolute_position - _current_position; //Investigate
    if (relative_distance < 0){
        _dir = -1;
    }
    else if(relative_distance > 0){
        _dir = 1;
    }

    if (abs(relative_distance)/2 < deceleration_distance){
        deceleration_distance = abs(relative_distance)/2;
    }
    new_move = true;
}

void Stepper::out_ofRange(){
  Serial.println("Move not possible due to calibration range restrictions");
}

void Stepper::move_relative(double relative_steps)
{
  setup_move(_current_position + relative_steps);
  digitalWrite(nEnable_pin,LOW);
  while(!move()){}
  digitalWrite(nEnable_pin,HIGH);
}

void Stepper::move_absolute(uint32_t position){
  setup_move(position);
  digitalWrite(nEnable_pin,LOW);
  while(!move()){}
  digitalWrite(nEnable_pin,HIGH);
}

bool Stepper::buttonPressed(int btn_pin,bool &btn_state){
  bool* last_state = &btn_state;
  int endstop_pin = btn_pin;
  int endstop_State = digitalRead(endstop_pin);
  lastDebounceTime = millis();
  if(endstop_State != *last_state){
    while((millis() - lastDebounceTime) < DEBOUNCE_DELAY){
      int _read = digitalRead(endstop_pin);
      if(_read != *last_state){
        lastDebounceTime = millis();
        *last_state = _read;
      }
      *last_state = _read;
    }
    Serial.println("trigger");
    return true;
  }
  return false;
}

bool Stepper::move()
{
    unsigned int period_last_step;
    unsigned int current_time_US;
    int distance_2_target;

    if(_current_position == absolute_position)
    {   
        return true;
    }

    if(buttonPressed(endstop1_pin, endstop1_lastState))
    {   
        return true;
    }

    if(buttonPressed(endstop2_pin, endstop2_lastState))
    {   
        return true;
    }

    digitalWrite(direction_pin,abs(min(0,_dir)));

    if(new_move){
        multiplier = 1;
        last_move_time_US = micros();
        new_move = false;
    }
    current_time_US = micros();
    period_last_step = current_time_US - last_move_time_US;

    if(period_last_step < this_move_period){
        return false;
    }
    last_move_time_US = micros();

    distance_2_target = abs(absolute_position - _current_position);

    if(distance_2_target == deceleration_distance){
        multiplier = -1;
    }
    if(this_move_period < running_period_US){
        this_move_period = running_period_US;
        multiplier = 0;
    }

    
    digitalWrite(step_pin,HIGH);
    delayMicroseconds(2);
    this_move_period = this_move_period*(1.0-multiplier*(_accel/1E12)*this_move_period*this_move_period);
    _current_position = _current_position + _dir;

    digitalWrite(step_pin,LOW);

    last_move_time_US = current_time_US;

    if(_current_position == absolute_position)
    {   
        return true;
    }
    return false;
}