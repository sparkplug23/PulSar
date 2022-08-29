

// ////////////////////////////////////////////////////////////////////////////////////////////////////
// // Submitted by benmprojects to handle "beep" commands. Split
// // incoming String by separator, return selected field as String
// // Original source: https://arduino.stackexchange.com/a/1237
// String getSubtringField(String data, char separator, int index)
// {
//   int found = 0;
//   int strIndex[] = {0, -1};
//   int maxIndex = data.length();

//   for (int i = 0; i <= maxIndex && found <= index; i++)
//   {
//     if (data.charAt(i) == separator || i == maxIndex)
//     {
//       found++;
//       strIndex[0] = strIndex[1] + 1;
//       strIndex[1] = (i == maxIndex) ? i + 1 : i;
//     }
//   }
//   return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
// }
