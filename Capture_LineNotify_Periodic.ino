time_t next_alarm_calc(){
  time_t _cur_alarm   = secs_of_day(hour(), minute(), second());
  time_t _next_alarm  =  ( int( _cur_alarm / (LINE_EVERY_PERIOD*60) ) + 1 ) * LINE_EVERY_PERIOD*60 ;

  int h_start = String(LINE_START).substring(0,2) .toInt();
  int m_start = String(LINE_START).substring(3)   .toInt();
  int h_stop  = String(LINE_STOP) .substring(0,2) .toInt();
  int m_stop  = String(LINE_STOP) .substring(3)   .toInt();

  time_t line_start_alarm  = secs_of_day( h_start, m_start, 0);
  time_t line_stop_alarm   = secs_of_day( h_stop, m_stop, 0);

  if( _next_alarm < line_start_alarm - 10 || _next_alarm > line_stop_alarm + 10 )  _next_alarm = line_start_alarm;  
  return _next_alarm;
}

void CaptureScreen_LineNotify_Periodic(){
  time_t cur_alarm   = secs_of_day(hour(), minute(), second());
  time_t next_alarm  = next_alarm_calc();



  BlynkGO.alarmOnce( /* เวลานาฬิกาที่ต้องการ alarm ในหน่วยวินาทีของวัน */, 
                     /* ชื่อฟังกชั่นที่ต้องการเรียกให้ทำงาน */ );
  
  // ตั้งเวลาให้ Alarm รอบถัดไป ตามเวลาที่กำหนด เพื่อให้เรียก CaptureScreen_LineNotify_Periodic() ขึ้นมาทำงาน
  BlynkGO.alarmOnce(next_alarm, CaptureScreen_LineNotify_Periodic);
  if( cur_alarm % (LINE_EVERY_PERIOD*60)==0 ) {
    Serial.printf("[Alarm] %02d:%02d:%02d\n", hour(cur_alarm), minute(cur_alarm), second(cur_alarm));

    /**************************************
     * หากจะให้ตั้งเวลาให้ทำอะไรให้เพิ่มตรงนี้ ....
     **************************************/
    // จำลองสุ่มค่า อุณหภูมิ และความชื้น  (หากทดลองจริงให้อ่านค่าจาก sensor ที่ใช้)
    float temp  = random(2000,3000)/100.0; 
    float humid = 65.5; //random(2000,6000)/100.0;

    // สร้างข้อความที่จะส่ง LINE Notify
    String message = StringX::printf("\n[%02d:%02d:%02d]\nTemp : %.2f\nHumid : %.2f", 
                          hour(cur_alarm), minute(cur_alarm), second(cur_alarm), temp, humid);

    myapp.hidden(false);    // screen ที่ต้องการจับภาพ ให้ไม่ซ่อน
    myapp.toForeground();   // screen ที่ต้องการ ให้เลื่อนมาอยู่เป็น layer บนสุด    
    if(BlynkGO.capture("SPIFFS://my_capture.png")){ // จับภาพหน้าจอ ลงบน SPIFFS
      LINE.notifyPicture(message, SPIFFS, "/my_capture.png" );  // สง LINE ด้วยภาพหน้าจอออกไป
    }else{
      LINE.notify(message);
    }
  }else{
    Serial.printf("[Next Alarm] %02d:%02d:%02d\n", hour(next_alarm), minute(next_alarm), second(next_alarm));
  }
}
