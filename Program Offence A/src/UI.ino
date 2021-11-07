void _UI::check(int num){
    if(!touchFlag[num]){
        if(touch[num]){
            switchingFlag[num]=true;
            touchFlag[num]=true;
        }
    }else{
        if(!touch[num]){
            counter[num]=0;
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
        // active=0;
        active=!active;
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
    for(int i=0; i<=3; i++){
        switchingFlag[i]=false;
    }
}