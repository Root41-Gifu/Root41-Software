import sensor, image, time, math

threshold_index = 0 # 0 for red, 1 for green, 2 for blue
#u may wish to tune them...
colorcode1 = [(10, 90, 15, 60, 25, 45)]
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
    exist=0
    area=0
    area_box=0
    degree=0
    front_distance=0
    behind_distance=0
    x_distance=0
    y_distance=0
    distance=0
    x_closs1=0
    y_closs1=0
    x_closs2=0
    y_closs2=0
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

        xc=blob.cx()-160
        yc=120-blob.cy()

        myball.area_box=blob.w()*blob.h()

        #範囲外排除
        if  xc<=x_detectWidth:
            if yc<=y_detectHeight:
                myball.exist=1

        #角度（横軸は-160~160、縦軸は-120~120）
        myball.degree_box=math.degrees(math.atan2(xc,yc))
        if myball.area_box>myball.area:
            myball.area=myball.area_box
            myball.degree=myball.degree_box

        #物体の距離計算
        if myball.degree<=90:
            if myball.degree>=-90:
                if myball.degree>0:
                    myball.x_distance=xc
                    myball.y_distance=yc
                else:
                    myball.x_distance=abs(xc)
                    myball.y_distance=yc
            else:
                myball.x_distance=abs(xc)
                myball.y_distance=abs(yc)
        else:
            myball.x_distance=xc
            myball.y_distance=abs(yc)

        myball.distance=math.sqrt(myball.x_distance**2+myball.y_distance**2)

        ##交点計算
        ##円の公式:x^2+y^2=radius^2
        ##線の公式:y=m*x+k
        r=camera.radius
        if xc!=0:
            m=yc/xc
        m=1
        a=1+math.pow(m,2)
        b=0
        c=-(math.pow(r,2))
        d=math.pow(b,2)-4*a*c

        r=50
        #print(a)
        #print(b)
        #print(c)
        #print(d)
        if d>0:
            x1 =(-b+math.sqrt(math.pow(b,2)-4*a*c)/2*a)
            y1=m*x1
            x1=int(x1)
            y1=int(y1)
            ##x2 =(-b-math.sqrt(math.pow(b,2)-4*a*c)/2a)
            ##x1=(-b+math.sqrt(math.pow(b,2)-4*a*c)/2a)
            #img.draw_circle(x1-160,y1-120,5,color=(255,255,255),thickness=1,fill=False)
            print(x1)
            print(y1)

        line_tuple=[160,120,xc,yc]
        if myball.distance<=100:
            img.draw_line ((160,120,blob.cx(),blob.cy()),  color=(255,0,0))
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
