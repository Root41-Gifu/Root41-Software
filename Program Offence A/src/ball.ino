_Ball::_Ball(){
    for(int i=0; i<=15; i++){
        vectorX[i]=sin(i*22.5*PI/180);
        vectorY[i]=cos(i*22.5*PI/180);
    }
}

void _Ball::read(void){
    // for(int i=0; i<32; i++){
        digitalWrite(PB5,LOW);
        int data = SPI.transfer(0); 
        digitalWrite(PB5, HIGH);
        if(data<16){
            readp=data;
        }else{
            value[readp]=data;
        } 
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