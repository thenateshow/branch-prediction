# branch-prediction
This project tests different methods of branch prediction on branch traces. This includes: 
  1. Always Taken
  2. Never Taken
  3. Bimodal Single
  4. Bimodal Double
  5. Gshare
  6. Tournament-style
  7. Branch Target Buffer

To test the predictions, there are sample traces in the /compressed-traces folder. Unzip them to use them when running.

To run all predictions, the Main function in the driver.cpp file will run each one and print the output to the output.txt file. Move the trace files to the same directory as the driver.cpp file.
