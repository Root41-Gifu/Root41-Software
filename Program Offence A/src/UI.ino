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
    if(mode==4){
        if(switchingFlag[0]){
            select=true;
            submode=1;
        }
        if(switchingFlag[1]){
            if(select){
                submode--;
            }else{
                mode--;
            }   
        }
        if(switchingFlag[2]){
            if(select){
                submode++;
            }else{
                mode++;
            }
        }
        if(switchingFlag[3]){
            active=false;
        }
    }else{
        if(switchingFlag[0]){
            select=false;
            active=!active;
        }
        if(switchingFlag[1]){
            mode--;
        }
        if(switchingFlag[2]){
            mode++;
        }
        if(switchingFlag[3]){
            active=false;
        }
    }
    for(int i=0; i<=3; i++){
        switchingFlag[i]=false;
    }
    if(mode<0){
        mode=7;
    }else if(mode>7){
        mode=0;
    }
}

void _UI::LCDdisplay(void){
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);

    if(UI.mode==0){
        if(!active){
            if((millis()/300)%3==0){
                display.println("Starting");
            }else if((millis()/300)%3==1){
                display.println("Starting.");
            }else if((millis()/300)%3==2){
                display.println("Starting..");
            }
        }else{
            display.println("Starting..");
        }
        display.setCursor(0,17);
        display.setTextSize(1);
    }else if(UI.mode==1){
        display.println("Offence");
    }else if(UI.mode==2){
        display.println("Defence");
    }else if(UI.mode==3){
        display.println("Gyro");
    }else if(UI.mode==4){
        display.println("Kicker");
    }else if(UI.mode==5){
        display.println("Line");
    }else if(UI.mode==6){
        display.println("Motor");
    }else if(UI.mode==7){
        display.println("Camera");
    }
    display.setCursor(0,17);
    display.print("Battery: ");
    display.print(String((float)Battery,1));
    display.println(" V");
    display.drawLine(0,15,127,15,WHITE);
    // 描画バッファの内容を画面に表示
    display.display();
}

void _UI::Errordisplay(int code){
    if(code!=0){
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        if(code==1){
            display.println("EMERGENCY!");
            display.setCursor(0,17);
            display.print("CODE:");
            display.print(code);
        }
        display.drawLine(0,15,127,15,WHITE);
    }
    display.display();
}