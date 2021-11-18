_Ball::_Ball(){
    for(int i=0; i<=15; i++){
        vectorX[i]=sin(i*22.5*PI/180);
        vectorY[i]=cos(i*22.5*PI/180);
    }
}

void _Ball::read(void){
    // // for(int i=0; i<32; i++){
    //     digitalWrite(PB5,LOW);
    //     int data = SPI.transfer(0); 
    //     digitalWrite(PB5, HIGH);
    //     // if(data<16){
    //     //     readp=data;
    //     // }else{
    //     //     value[readp]=data;
    //     // } 
    // value[0]=data;
    // // }
    SPI.begin();
    int data;
    unsigned long starTime=micros();
    digitalWrite(PB5, LOW);
    // SPI.transfer(-);
    for(int i=0; i<32; i++){
      data = SPI.transfer(0);
      delayMicroseconds(10);
      if(data<16){
        readp=data;
      }else if(i!=0){
        value[readp]=data;
      }
    }
    digitalWrite(PB5,HIGH);
    unsigned long endTime=micros();
    SPI.end();
    // if(data==0){
    //   readp=0;
    //   readp++;
    // }else{
    //   if(data>10){//誤動作の値削除
    //     value[readp-1]=data;
    //   }
    //   readp++;
    //   if(readp>16){
    //     readp=0;
    //   }
    // }
}

void _Ball::calcDistance(void){
    for(int i=0; i<=15; i++){
        if(value[i]<0){
            //近距離
            dist[i]=map(value[i],0,0,0,0);
        }else if(value[i]<0){
            //中距離
            dist[i]=map(value[i],0,0,0,0);
        }else if(value[i]<0){
            //遠距離
            dist[i]=map(value[i],0,0,0,0);
        }else if(value[i]<0){
            //範囲外のものを削除
            dist[i]=0;
        }
    }
}

void _Ball::calcDirection(void){
    vectortX+=vectorX[max[0]]+vectorX[max[1]]*0,3+vectorX[max[2]]*0.1;
    vectortX+=vectorX[max[0]]+vectorX[max[1]]*0,3+vectorY[max[2]]*0.1;

    degree=atan2(vectortX,vectortY)*180/PI;
}

void _Ball::average(void){
    if(millis()-averageTimer>50){
        max_average=0;
        for(int i=0; i<BALL_NUM; i++){
            if(averageCounter[max_average]<averageCounter[i]){
                max_average=i;
            }
        }
        for(int i=0; i<BALL_NUM; i++){
            averageCounter[i]=0;
        }
        averageTimer=millis();
    }else if(millis()){
        averageCounter[max[0]]++;
    }
}

void _Ball::calc(void){
    //簡単な方向、距離の分割プログラム
    if(dist[max[0]]<0){
        //1分割目
        move_degree=move[0][max[0]];
    }else if(dist[max[0]]<0){
        //2分割目
        move_degree=move[1][max[0]];
    }else if(dist[max[0]]<0){
        //3分割目
        move_degree=move[2][max[0]];
    }
}

void _Ball::LPF(void){
    float k;
    for(int i=0; i<16; i++){
        if(abs(value[i]-LPF_value[i])>30){
            k=0.1;//
        }else{
            k=0.05;
        }
        LPF_value[i] += k * (value[i] - LastLPF[i]);
        LastLPF[i] = LPF_value[i];
    }
}