_Line::_Line(){
    for(int i=0; i<LINE_NUM; i++){
        value[i]=false;
    }
}

void _Line::read(void){
    for(int i=0; i<LINE_NUM; i++){
        Wire.requestFrom(0x01, 5);//アドレスは変えてね

        int byteCounter;
        while (Wire.available()) {
            byte readValue = Wire.read();
            for(int i=0; i<8; i++){
                value[i+byteCounter*8]=readValue&(1<<i);//送られてきたバイト数のビット抽出するンゴ
            }
            byteCounter++;
        }
    }
}