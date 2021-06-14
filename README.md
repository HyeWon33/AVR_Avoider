# [AVR] Avoider


## 작동 환경
- Atmel Studio 7.0
- Atmega128
 
<img src = "https://user-images.githubusercontent.com/52944554/121841214-d7e5c180-cd18-11eb-8605-160ea5ca7e9b.PNG" width="20%">



## 사용한 것들
<img src="https://user-images.githubusercontent.com/52944554/121841664-be914500-cd19-11eb-95f7-88e48b52d0b8.png" width = "50%">

- 스텝 모터
- 스텝모터
- 초음파센서 
- 적외선센서
- LED

## 작동 설명
- 직진을 하다가 정면에서는 초음파센서 값을 받고 측면에서는 적외선 센서 값을 받아서 벽에 부딪치지 않고 회전하게 만들었습니다. 직진을 할 때는 LED가 전부 ON이 되고 죄회전을 할 때는 1~4번 LED만 ON이 됩니다.
- 스텝모터를 작동 할 때 고속 PWM모드를 사용했습니다.

## 작동 움짤
![ezgif com-video-to-gif (2)](https://user-images.githubusercontent.com/52944554/121842637-b20dec00-cd1b-11eb-9fcf-3ed1f7809744.gif)

## 단점
- 속도가 느립니다. 조금만 더 손보면 작동을 더 정교하게 할 수 있을 꺼 같습니다.
- 적외선 센서를 사용했습니다. 아트메가에 16비트 타이머/카운터가 1,3번 2개라서 1번은 비교일치를 하며 작동하기 위해서 쓰이고 3번은 초음파 센서를 위해 사용했습니다. 그러면서 측면 타이머/카운터를 위한 타이머/카운터가 없어서 값이 불안정하고 정교하지 않은거 같은 적외선 센서를 사용했습니다.  

## 장점
- 센서를 2개 사용해서 부드럽게 움직입니다.

