import pyb, ustruct, sensor, image, time, math
from pyb import UART

#SPIもろもろ

uart = UART(3, 19200)

threshold_index = 0

colorcode1 = [(8, 29, -35, -10, 23, 48)]
colorcode2 = [(0,0,0,0,0,0)]

middle=[143,120]

defo_roi=[0,0,320,240]

class CameraSet:
    radius=116


class GoalDetection:
    #ゴールの検知、角度、面積の算出をするクラス
    exist=0 #存在
    amount=0 #オブジェクトの数
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
    object_degree_total=0 #角度の合計

    mode=0

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
    xr_max=0
    xl_max=0
    def __init__(self):
        exist=0
        area=0

class GoalCalc:
    #GoalDetectionでの検知をもとに処理を主にするクラス
    degree=0 #総合的な角度
    degree_range=0 #範囲的な角度
    exist=0 #オブジェクトの有無
    amount=0 #オブジェクト数
    reliability=0 #信頼度
    disatance=0 #距離

#インスタンス生成
camera=CameraSet()
opponentsGoal = GoalDetection()
calc_opponentGoal=GoalCalc()


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_brightness(2)
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
    opponentsGoal.exist=0
    opponentsGoal.amount=0
    opponentsGoal.area=0
    opponentsGoal.degree=0
    opponentsGoal.area_total=0
    opponentsGoal.object_degree_total=0
    opponentsGoal.mode=0
    opponentsGoal.xl_max=0
    opponentsGoal.xr_max=0

    img.draw_circle(middle[0],middle[1],camera.radius,color=(255,255,255),thickness=1,fill=False)
    for blob in img.find_blobs([colorcode1[threshold_index]], roi= defo_roi,x_stride=10, y_stride=10,pixels_threshold=10, area_threshold=20, merge=True):
        opponentsGoal.amount+=1
        opponentsGoal.distance=0
        opponentsGoal.exist=1

        opponentsGoal.xc=blob.cx()-middle[0]
        opponentsGoal.yc=middle[1]-blob.cy()
        opponentsGoal.xl=blob.x()-middle[0]
        opponentsGoal.yl=middle[1]-blob.y()
        opponentsGoal.xr=(blob.x()+blob.w())-middle[0]
        opponentsGoal.yr=middle[1]-(blob.y()+blob.h())

        opponentsGoal.area=blob.w()*blob.h()
        opponentsGoal.area_total+=opponentsGoal.area

        opponentsGoal.exist=0

        #範囲外排除
        if  opponentsGoal.xc<=camera.radius:
            if opponentsGoal.yc<=camera.radius:
                opponentsGoal.exist=1

        if opponentsGoal.exist:

            #角度（横軸は-160~160、縦軸は-120~120）
            opponentsGoal.degree=math.degrees(math.atan2(opponentsGoal.xc,opponentsGoal.yc))

            #物体の距離計算
            if opponentsGoal.degree<=90:
                if opponentsGoal.degree>=-90:
                    if opponentsGoal.degree>0:
                        opponentsGoal.x_distance=opponentsGoal.xc
                        opponentsGoal.y_distance=opponentsGoal.yc
                    else:
                        opponentsGoal.x_distance=abs(opponentsGoal.xc)
                        opponentsGoal.y_distance=opponentsGoal.yc
                else:
                    opponentsGoal.x_distance=abs(opponentsGoal.xc)
                    opponentsGoal.y_distance=abs(opponentsGoal.yc)
            else:
                opponentsGoal.x_distance=opponentsGoal.xc
                opponentsGoal.y_distance=abs(opponentsGoal.yc)

            opponentsGoal.distance=int(math.sqrt(opponentsGoal.x_distance**2+opponentsGoal.y_distance**2))

            ##交点計算
            opponentsGoal.xcc=crossCheckX(camera.radius,opponentsGoal.xc,opponentsGoal.yc)
            opponentsGoal.ycc=crossCheckY(camera.radius,opponentsGoal.xc,opponentsGoal.yc)
            opponentsGoal.xlc=crossCheckX(camera.radius,opponentsGoal.xl,opponentsGoal.yl)
            opponentsGoal.ylc=crossCheckY(camera.radius,opponentsGoal.xl,opponentsGoal.yl)
            opponentsGoal.xrc=crossCheckX(camera.radius,opponentsGoal.xr,opponentsGoal.yr)
            opponentsGoal.yrc=crossCheckY(camera.radius,opponentsGoal.xr,opponentsGoal.yr)

            if opponentsGoal.xlc<opponentsGoal.xl_max:
                opponentsGoal.xl_max=opponentsGoal.xlc

            if opponentsGoal.xrc>opponentsGoal.xr_max:
                opponentsGoal.xr_max=opponentsGoal.xrc


            #角度算出
            opponentsGoal.l_distance=math.sqrt(math.pow(opponentsGoal.xlc,2)+math.pow(opponentsGoal.ylc,2))
            opponentsGoal.r_distance=math.sqrt(math.pow(opponentsGoal.xrc,2)+math.pow(opponentsGoal.yrc,2))
            opponentsGoal.innerp=opponentsGoal.xlc*opponentsGoal.xrc+opponentsGoal.ylc*opponentsGoal.yrc
            opponentsGoal.object_cos=opponentsGoal.innerp/(opponentsGoal.r_distance*opponentsGoal.l_distance)
            if abs(opponentsGoal.object_cos)<=1:
                opponentsGoal.object_degree=int(math.degrees(math.acos(opponentsGoal.object_cos)))
            else:
                opponentsGoal.object_degree=0
            opponentsGoal.object_degree_total+=opponentsGoal.object_degree

            #まとめるンゴ
            #if opponentsGoal.amount==1:
                #if opponentsGoal.obejct_degree_total>30:

            #映像系
            #img.draw_circle(opponentsGoal.xcc+160,120-opponentsGoal.ycc,5,color=(255,255,255),thickness=1,fill=False)
            #img.draw_circle(opponentsGoal.xlc+160,120-opponentsGoal.ylc,5,color=(255,255,255),thickness=1,fill=False)
            #img.draw_circle(opponentsGoal.xrc+160,120-opponentsGoal.yrc,5,color=(255,255,255),thickness=1,fill=False)

            #範囲規制（ゆるくしたいなら200でOk）
            #if opponentsGoal.distance<=200:
            img.draw_line ((middle[0],middle[1],blob.cx(),blob.cy()),  color=(255,0,0))
                #img.draw_line ((160,120,opponentsGoal.xlc+160,120-opponentsGoal.ylc), color=(255,255,0))
                #img.draw_line ((160,120,opponentsGoal.xrc+160,120-opponentsGoal.yrc),  color=(255,255,0))
            img.draw_edges(blob.min_corners(), color=(255,0,0))

    if opponentsGoal.xr_max>20:
        if opponentsGoal.xl_max<-20:
            opponentsGoal.mode=3
        else :
            opponentsGoal.mode=2
    elif opponentsGoal.xl_max<-20:
        opponentsGoal.mode=1


        #img.draw_keypoints([(blob.cx(), blob.cy(),          int(math.degrees(blob.rotation())))], size=20)
    print(opponentsGoal.xl_max)
    print(opponentsGoal.xr_max)
    print(opponentsGoal.mode)

    data=opponentsGoal.mode
    try:
        uart.write(ustruct.pack('B',data))
    except (OSError, RuntimeError) as err:
        pass
