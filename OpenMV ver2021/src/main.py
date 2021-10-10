import sensor, image, time, math

threshold_index = 0

colorcode1 = [(25, 90, 26, 89, 18, 50)]
colorcode2 = [(0,0,0,0,0,0)]

x_detectWidth=150
y_detectHeight=150

defo_roi=[0,0,320,240]

class CameraSet:
    radius=100


class GoalDetection:
    exist=0 #存在
    area=0 #面積
    area_total=0 #面積合計
    degree=0 #角度
    x_distance=0 #中央までの距離
    y_distance=0
    distance=0 #総合的な距離
    l_distance=0 #左角までの距離
    r_distance=0 #右角までの距離
    innerp=0 #内積
    object_cos=0 #内積からのcos値
    object_degree=0 #対象の角度（広さ的な意味の）

    #座標
    xc=0 ##対象の中心
    yc=0
    xcc=0 ##中心から伸ばした円状の交点
    ycc=0
    xl=0 ##対象の左側の座標
    yl=0
    xlc=0 ##xlからの交点
    ylc=0
    xr=0 ##対象の右側の座標
    yr=0
    xrc=0 ##xrからの交点
    yrc=0
    def __init__(self):
        exist=0
        area=0

camera=CameraSet()
myball = GoalDetection()

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock()

def crossCheckX(r,x,y):
    if x!=0:
        m=y/x
    else :
        m=0
    a=1+math.pow(m,2)
    b=0
    c=-(math.pow(r,2))
    d=math.pow(b,2)-4*a*c
    if d>0:
        if x>0:
            x1 =((-b+math.sqrt(math.pow(b,2)-4*a*c))/(2*a))
        else :
            x1 =((-b-math.sqrt(math.pow(b,2)-4*a*c))/(2*a))
            #x1=(math.sqrt(80000)/(2*2))
        y1=m*x1
        x1=int(x1)
        y1=int(y1)
    return x1

def crossCheckY(r,x,y):
    if x!=0:
        m=y/x
    else:
        m=0
    a=1+math.pow(m,2)
    b=0
    c=-(math.pow(r,2))
    d=math.pow(b,2)-4*a*c
    if d>0:
        if x>0:
            x1 =((-b+math.sqrt(math.pow(b,2)-4*a*c))/(2*a))
        else :
            x1 =((-b-math.sqrt(math.pow(b,2)-4*a*c))/(2*a))
        y1=m*x1
        x1=int(x1)
        y1=int(y1)
    return y1


while(True):
    clock.tick()
    img = sensor.snapshot()

    #リセット
    myball.exist=0
    myball.area=0
    degree=0
    degree_box=0

    img.draw_circle(160,120,camera.radius,color=(255,255,255),thickness=1,fill=False)
    for blob in img.find_blobs([colorcode1[threshold_index]], roi= defo_roi,x_stride=10, y_stride=10,pixels_threshold=20, area_threshold=5, merge=True):
        myball.distance=0
        myball.exist=1

        myball.xc=blob.cx()-160
        myball.yc=120-blob.cy()
        myball.xl=blob.x()-160
        myball.yl=120-blob.y()
        myball.xr=(blob.x()+blob.w())-160
        myball.yr=120-(blob.y()+blob.h())

        myball.area=blob.w()*blob.h()
        myball.area_total+=myball.area

        #範囲外排除
        if  myball.xc<=x_detectWidth:
            if myball.yc<=y_detectHeight:
                myball.exist=1

        #角度（横軸は-160~160、縦軸は-120~120）
        myball.degree=math.degrees(math.atan2(myball.xc,myball.yc))

        #物体の距離計算
        if myball.degree<=90:
            if myball.degree>=-90:
                if myball.degree>0:
                    myball.x_distance=myball.xc
                    myball.y_distance=myball.yc
                else:
                    myball.x_distance=abs(myball.xc)
                    myball.y_distance=myball.yc
            else:
                myball.x_distance=abs(myball.xc)
                myball.y_distance=abs(myball.yc)
        else:
            myball.x_distance=myball.xc
            myball.y_distance=abs(myball.yc)

        myball.distance=int(math.sqrt(myball.x_distance**2+myball.y_distance**2))

        ##交点計算
        myball.xcc=crossCheckX(camera.radius,myball.xc,myball.yc)
        myball.ycc=crossCheckY(camera.radius,myball.xc,myball.yc)
        myball.xlc=crossCheckX(camera.radius,myball.xl,myball.yl)
        myball.ylc=crossCheckY(camera.radius,myball.xl,myball.yl)
        myball.xrc=crossCheckX(camera.radius,myball.xr,myball.yr)
        myball.yrc=crossCheckY(camera.radius,myball.xr,myball.yr)

        #映像系
        img.draw_circle(myball.xcc+160,120-myball.ycc,5,color=(255,255,255),thickness=1,fill=False)
        img.draw_circle(myball.xlc+160,120-myball.ylc,5,color=(255,255,255),thickness=1,fill=False)
        img.draw_circle(myball.xrc+160,120-myball.yrc,5,color=(255,255,255),thickness=1,fill=False)
        line_tuple=[160,120,myball.xc,myball.yc]

        #範囲規制（ゆるくしたいなら200でOk）
        if myball.distance<=200:
            img.draw_line ((160,120,blob.cx(),blob.cy()),  color=(255,0,0))
            img.draw_line ((160,120,myball.xlc+160,120-myball.ylc), color=(255,255,0))
            img.draw_line ((160,120,myball.xrc+160,120-myball.yrc),  color=(255,255,0))
            img.draw_edges(blob.min_corners(), color=(255,0,0))

        #角度算出
        myball.l_distance=math.sqrt(math.pow(myball.xlc,2)+math.pow(myball.ylc,2))
        myball.r_distance=math.sqrt(math.pow(myball.xrc,2)+math.pow(myball.yrc,2))
        myball.innerp=myball.xlc*myball.xrc+myball.ylc*myball.yrc
        myball.object_cos=myball.innerp/(myball.r_distance*myball.l_distance)
        myball.object_degree=int(math.degrees(math.acos(myball.object_cos)))

        img.draw_keypoints([(blob.cx(), blob.cy(),          int(math.degrees(blob.rotation())))], size=20)
    print(myball.object_degree)
    print(myball.exist)
