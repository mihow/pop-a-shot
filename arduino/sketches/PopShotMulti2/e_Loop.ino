void loop()
{
  readSerial();
  
  hoop1.Update();
  hoop1.UpdateDisplay(tlc);
  
  hoop2.Update();
  hoop2.UpdateDisplay(tlc);
  
  writeDisplay();
}
