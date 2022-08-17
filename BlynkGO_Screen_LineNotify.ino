#include <BlynkGOv3.h>
#include <TridentTD_LineNotify.h>

#define LINE_TOKEN                "--------------------"    // Line Token
#define LINE_START                "07:00"                   // เวลาเริ่มส่งเพื่อส่ง Line Notify
#define LINE_STOP                 "17:30"                   // เวลาสิ้นสุดเพื่อส่ง Line Notify
#define LINE_EVERY_PERIOD         10                        // ส่ง Line Notify ทุกๆ กี่นาที (minutes)

IMG_DECLARE(img_bg);

GImage myapp(img_bg);
  GWiFiManager wifi_manager(myapp);
  GLabel   lb_clock(myapp);

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  myapp.align_center();
    wifi_manager.align(ALIGN_TOP_RIGHT,-5,-2);
    lb_clock.align(wifi_manager, ALIGN_LEFT, -10);
    
  LINE.setToken(LINE_TOKEN);
}

void loop() {
  BlynkGO.update();
}

NTP_SYNCED(){
  static GTimer timer;
  timer.setInterval(1000L, [](){
    lb_clock = StringX::printf("%02d:%02d:%02d", hour(), minute(), second());
  });
  
  static bool first_synced = true;
  if(first_synced){
    CaptureScreen_LineNotify_Periodic();
    first_synced = false;
  }
}
