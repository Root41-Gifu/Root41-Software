void _UI::check(int num){
    if(!touchflag[num]){
        if(counter[num]<30){
            if(touch[num]){
                counter[num]++;
            }else{
                counter[num]=0;
            }
        }else if(counter[num]==30){
            switchingFlag[num]=true;
            touchFlag[num]=true;
        }
    }else{
        if(!touch[num]){
            touchFlag[num]=false;
            longpressFlag[num]=false;
        }
        if(longpressFlag[num]>0){
            longpressFlag[num]=true;
        }
    }
}

void _UI::refrection(void){
    if(switchingFlag[0]){
        active=0;
    }
    if(switchingFlag[1]){
        mode--;
    }
    if(switchingFlag[2]){
        mode++;
    }
    if(switchingFlag[3]){
        active=1;
    }
}