void loop()
{
  readSerial();

  //  hoop1.Update();
  //  hoop1.UpdateDisplay(tlc);

  if (!gamesPaused) {
    for (int i = 0; i < numHoops; i++) {
      
      // Update all configured hoops
      hoops[i]->Update();
      hoops[i]->UpdateDisplay(tlc);
    }

    writeDisplay();
  }
}
