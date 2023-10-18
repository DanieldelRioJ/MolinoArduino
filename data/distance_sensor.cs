class DistanceSensor{

  private:
    uint8_t address;
    uint8_t shtdwnPin;

  public:

    DistanceSensor(uint8_t shtdwnPin, uint8_t i2caddr = VL6180X_DEFAULT_I2C_ADDR){
      address = i2caddr;
    }

    void shutdown(){

    }

    void wakeUp(){

    }

    void setup(){

    }

    void loop(){
      Serial.println(shtdwnPin);
    }


};