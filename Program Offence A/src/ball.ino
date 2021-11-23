_Ball::_Ball(){
    for(int i=0; i<BALL_NUM; i++){
        vectorX[i]=sin(radians(i*22.5));
        vectorY[i]=cos(radians(i*22.5));
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
    for(int i=0; i<33; i++){
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
    for(int i=0; i<BALL_NUM; i++){
        dist[i]=1;
    //     if(value[i]<0){
    //         //近距離
    //         dist[i]=map(value[i],0,0,0,0);
    //     }else if(value[i]<0){
    //         //中距離
    //         dist[i]=map(value[i],0,0,0,0);
    //     }else if(value[i]<0){
    //         //遠距離
    //         dist[i]=map(value[i],0,0,0,0);
    //     }else if(value[i]<0){
    //         //範囲外のものを削除
    //         dist[i]=0;
    //     }
    }
}

void _Ball::calcDirection(void){
    vectortX=vectorX[max_average[0]]+vectorX[max_average[1]]*0.3+vectorX[max_average[2]]*0.1;
    vectortY=vectorY[max_average[0]]+vectorY[max_average[1]]*0.3+vectorY[max_average[2]]*0.1;

    degree=degrees(atan2(vectortX,vectortY));
    //右が（0~180）、左が(-180~0)
}

void _Ball::average(void){
    if(millis()-averageTimer>50){
        int lastmax_average[3];
        lastmax_average[0]=max_average[0];
        lastmax_average[1]=max_average[1];
        lastmax_average[2]=max_average[2];
        max_average[0]=100;
        max_average[1]=100;
        max_average[2]=100;
        for(int i=0; i<BALL_NUM; i++){
            if(averageCounter[max_average[0]]<averageCounter[i]||max_average[0]==100){
                max_average[2]=max_average[1];
                max_average[1]=max_average[0];
                max_average[0]=i;
            }else if(averageCounter[max_average[1]]<averageCounter[i]||max_average[1]==100){
                max_average[2]=max_average[1];
                max_average[1]=i;
            }else if(averageCounter[max_average[2]]<averageCounter[i]||max_average[2]==100){
                max_average[2]=i;
            }
        }
        
        if(averageCounter[max_average[0]]<averageCounter[16]){
            max_average[0]=100;//ない判定
        }else if(averageCounter[max_average[0]]==0){
            max_average[0]=lastmax_average[0];
        }
        for(int i=0; i<BALL_NUM+1; i++){
            averageCounter[i]=0;
        }
        averageTimer=millis();
    }else{
        if(max[0]!=100){
            averageCounter[max[0]]+=4;
            if(max[1]!=100){
                averageCounter[max[1]]+=2;
                if(max[2]!=100){
                    averageCounter[max[2]]+=1;
                }
            }
        }else{
            averageCounter[16]++;
        }
    }
}

void _Ball::calc(void){
    //簡単な方向、距離の分割プログラム
    int _degree;
    int move_16[3][16]={
        {0,22,45,68,90,112,135,157,180,202,225,247,270,292,315,337},
        {0,22,45,68,90,112,135,157,180,202,225,247,270,292,315,337},
        {0,22,45,68,90,112,135,157,180,202,225,247,270,292,315,337}
    };
    _degree=move_16[dist[max[0]]][max[0]];
    move_degree=_degree;
}

void _Ball::LPF(void){
    float k;
    //LPF調整ファイト
    for(int i=0; i<16; i++){
        if(abs(value[i]-LPF_value[i])>30){
            k=0.07;//0.1
            if(value[i]-LPF_value[i]<-30){
                k=0.15;//0.15
            }
        }else{
            k=0.03;//0.07
        }
        LPF_value[i] += k * (value[i] - LastLPF[i]);
        LastLPF[i] = LPF_value[i];
    }
}

int _Ball::adjustValue(int number,int originalValue){
    //0,100~
    //1,110~
    //2,120~
    //3,125~ ときどき80~もある
    //4,130~
    //5,140
    //8,110~
    //9,110~
    //10,110~ 真正面80~
    //11,100~
    //12.120~
    //13.120~ 真正面0も
    //14.120~
    int adjustment;
    switch (number){
        case 0:
            if(originalValue>100){
                adjustment=originalValue+10;
                break;
            }
        case 8: 
            if(originalValue>80){
                adjustment=originalValue+20;
                break;
            }
        case 11:
            if(originalValue<110&&originalValue>60){
                adjustment=originalValue-20;
                if(value[8]>120){
                    adjustment-=15;
                }
                break;
            }
        case 12:
            if(originalValue<110&&originalValue>60){
                adjustment=originalValue-20;
                if(value[8]>120){
                    adjustment-=15;
                }
                break;
            }
        case 14:
            if(originalValue>90){
                adjustment=originalValue+35;
            }
        default:
            adjustment=originalValue;
            break;
    }
    return adjustment;
}