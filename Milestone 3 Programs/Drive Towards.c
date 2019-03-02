#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    IR_SHORT,      sensorReflection)
#pragma config(Sensor, in2,    IR_LONG,      sensorReflection)
#pragma config(Sensor, dgtl3,  button1,        sensorTouch)
#pragma config(Sensor, dgtl4,  sonarSensor,    sensorSONAR_cm)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           motor1,        tmotorVex393_HBridge, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port2,           motor2,        tmotorVex393_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port10,          motor3,        tmotorVex393_HBridge, openLoop, encoderPort, I2C_1)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

typedef enum T_State {
    LIGHT_OFF = 0,
    LIGHT_ON
};

short LONG_IR_SENSOR_THRESHOLD = 600;
short SHORT_IR_SENSOR_THRESHOLD = 2000;

const int beacon_threshold = 500;


// Perform processing of measurements.
// Should be called with rate of at least 20 Hertz for proper detection of puck.
int monitorLightShort()
{
    // Static variables are a special class of variables that maintain
    // their values between subsequent calls to a function.  The intialization
    // values are only stored in the variables when the function is first called.
    // After that, the values that were in the variable at the end of the last time
    // the function is called will be stored in the variable when the function
    // execution is started.
    static int minLevelIR1 = 4096;    // Minimum light level seen by IR sensor 1
    static int maxLevelIR1 = 0;            // Maximum light level seen by IR sensor 1
    static int diffLevelIR1 = 0;        // Delta between maximum and minimum seen in last 0.1 seconds
    
    int lightLevel1 = SensorValue[IR_SHORT];
    
    // Check if 100 msecs have elapsed.
    if ( time1[T1] > 100 )  {
        
        // 100 msecs have elapsed.  Compute delta of light level.
        diffLevelIR1 = maxLevelIR1 - minLevelIR1;
        
        // Reset calculation for next 100 msecs.
        maxLevelIR1 = 0;
        minLevelIR1 = 4096;
        clearTimer(T1);
        
    } else {
        
        // Check for new minimum/maximum light levels.
        if ( lightLevel1 < minLevelIR1 ) {
            minLevelIR1 = lightLevel1;
        } else if ( lightLevel1 > maxLevelIR1 ) {
            maxLevelIR1 = lightLevel1;
        }
    }
    
    return(diffLevelIR1);
} // end monitorLightShort

// Perform processing of measurements.
// Should be called with rate of at least 20 Hertz for proper detection of puck.
int monitorLightLong()
{
    // Static variables are a special class of variables that maintain
    // their values between subsequent calls to a function.  The intialization
    // values are only stored in the variables when the function is first called.
    // After that, the values that were in the variable at the end of the last time
    // the function is called will be stored in the variable when the function
    // execution is started.
    static int minLevelIR1 = 4096;    // Minimum light level seen by IR sensor 1
    static int maxLevelIR1 = 0;            // Maximum light level seen by IR sensor 1
    static int diffLevelIR1 = 0;        // Delta between maximum and minimum seen in last 0.1 seconds
    
    int lightLevel1 = SensorValue[IR_LONG];
    
    // Check if 100 msecs have elapsed.
    if ( time1[T1] > 100 )  {
        
        // 100 msecs have elapsed.  Compute delta of light level.
        diffLevelIR1 = maxLevelIR1 - minLevelIR1;
        
        // Reset calculation for next 100 msecs.
        maxLevelIR1 = 0;
        minLevelIR1 = 4096;
        clearTimer(T1);
        
    } else {
        
        // Check for new minimum/maximum light levels.
        if ( lightLevel1 < minLevelIR1 ) {
            minLevelIR1 = lightLevel1;
        } else if ( lightLevel1 > maxLevelIR1 ) {
            maxLevelIR1 = lightLevel1;
        }
    }
    
    return(diffLevelIR1);
} // end monitorLightLong


void turn()
{
      resetMotorEncoder(motor1);
      while(getMotorEncoder(motor1) <= 100)
      	{
      		//Slow motor down as it approaches
     			motor[motor1] = 20;
      		motor[motor2] = -20;
      	}//while
      //stops motors
      motor[motor1] = 0;
      motor[motor2] = 0;
} // end turn

void lower_arm()
{
	resetMotorEncoder(motor3);
			motor[motor3] = -20 ;
				delay(800);
				motor[motor3] = 0;
				delay(500);
				motor[motor1] = -30;
        motor[motor2] = -30;
        delay(600);
    /*    while(getMotorEncoder(motor3) >= -500)
        	{
            motor[motor1] = -30;
            motor[motor2] = -30;
          }//while
          */
          motor[motor1] = 0;
          motor[motor2] = 0;

}// end lower_arm

void fast_drive()
{
            while(SensorValue(sonarSensor) > 40  || SensorValue(sonarSensor) == -1)
            	{
            	motor[motor1] = 63;
            	motor[motor2] = 63;
        			}
        motor[motor1] = 0;
        motor[motor2] = 0;
} // end fast_drive


void slow_drive()
{
            while(SensorValue(sonarSensor) > 15  || SensorValue(sonarSensor) == -1)
            	{
            	motor[motor1] = 15;
            	motor[motor2] = 15;
        			}
        motor[motor1] = 0;
        motor[motor2] = 0;
} // end slow_drive


task main ()
{
    T_State robot_state = LIGHT_OFF;
    int  beacon_level;
    
    while( true ) {
        
        // Update sensor values (must be called at least 20 times a second for proper performance).
        beacon_level = monitorLight();
        
        switch( robot_state ) {
            case LIGHT_OFF:
                SensorValue[StateLED] = 0;
                if ( beacon_level > beacon_threshold ) {
                    robot_state = LIGHT_ON;
                } else {
                    robot_state = LIGHT_OFF;
                }
                break;
            case LIGHT_ON:
                SensorValue[StateLED] = 1;
                if ( beacon_level > beacon_threshold ) {
                    robot_state = LIGHT_ON;
                } else {
                    robot_state = LIGHT_OFF;
                }
                break;
            default:
                // This should never happen.
                robot_state = LIGHT_OFF;
                
        } // switch( robot_state)
        
    }  // while(true)


}//main
