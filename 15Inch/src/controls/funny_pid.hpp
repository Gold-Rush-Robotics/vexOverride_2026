// This is some test code. It is a semi-working PID with integral decay.

int whenStarted1() {
  return 0;
}

float fclamp(float a,float mn,float mx) {
  float var = a;
  if (var > mx) {
    var = mx;
  }
  if (var < mn) {
    var = mn;
  }
  return var;
}

float fabs(float a) {
  if (a < 0) {
    return -a;
  } else {
    return a;
  }
}


int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  whenStarted1();
  Motor8.spin(forward);
  //Motor6.spin(forward);
  // PID
  float kp = 20;
  float ki = 20;
  float kd = 10;
  // End of PID
  float dt = 0.02;
  float errorlast = 0;
  float integralhandler = 0;
  float idecay = 2;
  while (Controller1.ButtonA.pressing() == false) {
    wait(dt,seconds);
    float antigarbagecontrollerinput = Controller1.Axis2.position();
    if (fabs(antigarbagecontrollerinput) < 10) {
      antigarbagecontrollerinput = 0;
    }
    float target = antigarbagecontrollerinput + 0;
    float pos = Motor8.velocity(percent);
    float error = target - pos;

    integralhandler += fclamp(dt*error*10,-1000,1000);
    integralhandler = integralhandler/(1+(dt*idecay));
    float derivative = fclamp((error - errorlast)/dt,-7,7);

    float vel = error*kp/180 + ki*integralhandler/100 + kd*derivative/100;

    Motor8.setVelocity(fclamp(vel,-100,100),percent);
    //Motor6.setVelocity(fclamp(vel,-100,100),percent);

    errorlast = error;
  }
}
