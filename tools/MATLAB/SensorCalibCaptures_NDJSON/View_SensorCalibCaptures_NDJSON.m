filename = 'sensor_calib_captures.json';     % or .json


%%

% In the folder with both .m files:
[T, By] = LoadSensorCalibNDJSON(filename);  % NDJSON
PlotSensorsByType(By,'Temperature');                             % all DS18B20s


%%


