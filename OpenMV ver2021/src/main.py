import sensor, image, time, math

threshold_index = 0 # 0 for red, 1 for green, 2 for blue
#u may wish to tune them...
colorcode1 = [(25, 90, 26, 89, 18, 50)]
colorcode2 = [(0,0,0,0,0,0)]
x_keypoint=0
y_keypoint=0

x_detectWidth=0
y_detectHeight=0

defo_roi=[0,0,320,240]

degree=math.degrees(math.atan2(x_keypoint,y_keypoint))

class CameraSet:
    radius=100


class GoalDetection:
    exist=0 ##存在
    area=0 ##面積
    area_box=0 #わからん
    degree=0 #角度
    front_distance=0
    behind_distance=0
    x_distance=0
    y_distance=0
    distance=0

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
    def calc(self):
        degree=0

camera=CameraSet()
myball = GoalDetection()

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock()

# Only blobs that with more pixels than "pixel_threshold" and more area than "area_threshold" are
# returned by "find_blobs" below. Change "pixels_threshold" and "area_threshold" if you change the
# camera resolution. "merge=True" merges all overlapping blobs in the image.

#def GoalDelection::__init__():
    #exist=0
    #area=0

#def GoalDelection::existCheck():

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
    myball.exist=0
    myball.area=0
    deg_count=0
    degree=0
    degree_box=0

    img.draw_circle(160,120,camera.radius,color=(255,255,255),thickness=1,fill=False)
    for blob in img.find_blobs([colorcode1[threshold_index]], roi= defo_roi,x_stride=20, y_stride=20,pixels_threshold=200, area_threshold=200, merge=True):
        myball.distance=0
        myball.exist=1

        myball.xc=blob.cx()-160
        myball.yc=120-blob.cy()
        myball.xl=blob.x()-160
        myball.yl=120-blob.y()
        myball.xr=(blob.x()+blob.w())-160
        myball.yr=120-(blob.y()+blob.h())

        myball.area_box=blob.w()*blob.h()

        #範囲外排除
        if  myball.xc<=x_detectWidth:
            if myball.yc<=y_detectHeight:
                myball.exist=1

        #角度（横軸は-160~160、縦軸は-120~120）
        myball.degree_box=math.degrees(math.atan2(myball.xc,myball.yc))
        #if myball.area_box>myball.area:
            #myball.area=myball.area_box
            #myball.degree=myball.degree_box

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
        #img.draw_line(blob.major_axis_line(), color=(0,255,0))
        #img.draw_line(blob.minor_axis_line(), color=(0,0,255))

        # These values are stable all the time.
        # Note - the blob rotation is unique to 0-180 only.
        img.draw_keypoints([(blob.cx(), blob.cy(),          int(math.degrees(blob.rotation())))], size=20)




    #img.draw_circle（160,120,80,thickness = 1,color=(255,255,255),thickness = 1,fill=false）
    #print(clock.fps())
    #print(goal_position)
    #img.draw_rectangle(blob.rect())
    #img.draw_cross(blob.cx(), blob.cy())
    #print(myball.degree)
    #print(myball.exist)
    #print(myball.distance)
