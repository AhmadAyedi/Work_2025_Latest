Thanks for downloading our LIN data pack!

This pack contains the following:

1) MF4 log file data recorded from a HELLA IBS (Intelligent Battery Sensor) 24V with the CANedge 
2) LDF file for decoding the data by e.g. finalizing the MF4 (using our MF4 to MF4 converter) and loading it in Vector tools
3) DBC file (partial) for decoding the data in e.g. asammdf or the CANedge MF4 decoders
4) Transmit list that can be loaded into your CANedge Configuration File via the 'partial config loader' tool (see the CANedge Intro)
- Note that this assumes a specific battery type for the configuration, which will need to be changed based on the battery 
- If you want to 'listen' as a LIN slave rather than act as the LIN master with your CANedge, simply remove the LIN transmit list 

You can open the MF4 log files e.g. in below software tools and use the DBC files to decode the data.

1) asammdf GUI: Good starting point for opening, DBC decoding and plotting the data
- Introduction: https://www.csselectronics.com/pages/asammdf-gui-api-mdf4-mf4

2) Python API: Automate the processing of the data
- Introduction: https://www.csselectronics.com/pages/python-can-bus-api

3) Grafana dashboards: Create custom dashboard visualizations of the data
- Introduction: https://www.csselectronics.com/pages/telematics-dashboard-open-source

4) MF4 converters: Ideal if you want to work with the log files in e.g. Vector/PEAK tools
- Introduction: https://www.csselectronics.com/pages/mdf4-converters-mf4-asc-csv

5) MF4 decoders: Easily DBC decode your data to CSV files or Parquet data lakes
- Introduction: https://www.csselectronics.com/pages/mdf4-decoders-dbc-mf4-parquet-csv

If any questions, feel free to contact us: https://www.csselectronics.com/pages/contact-us

best,
CSS Electronics