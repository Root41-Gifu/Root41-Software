_Ball::_Ball(){
    for(int i=0; i<=15; i++){
        vectorX[i]=sin(i*22.5*PI/180);
        vectorY[i]=cos(i*22.5*PI/180);
    }
}

void _Ball::read(void){

}

void _Ball::calcDistance(void){
    for(int i=0; i<=15; i++){
        if(val[i]<0){
            //近距離
            dist[i]=map(val[i],0,0,0,0);
        }else if(val[i]<0){
            //中距離
            dist[i]=map(val[i],0,0,0,0);
        }else if(val[i]<0){
            //遠距離
            dist[i]=map(val[i],0,0,0,0);
        }else if(val[i]<0){
            //範囲外のものを削除
            dist[i]=0;
        }
    }
}