%% Setup
cfg.baseURL     = 'http://192.168.3.70:8086';
cfg.db          = 'openhab_meadows';
cfg.rp          = 'autogen';     % important for your DB
cfg.username    = '';            % if needed
cfg.password    = '';            % if needed
cfg.stepSeconds = [];             % [] to skip resampling

% time window (UTC)
tEnd   = datetime('now','TimeZone','UTC');
tStart = tEnd - minutes(300);

% your measurements
meas = [
"SENSOR_3DPrinterFrontRight00_Temperature"
"SENSOR_3DPrinterFrontRight25_Temperature"
"SENSOR_3DPrinterFrontRight50_Temperature"
"SENSOR_3DPrinterFrontRight75_Temperature"
"SENSOR_3DPrinterFrontRight100_Temperature"
"SENSOR_3DPrinterFrontTopLeft_Temperature"
"SENSOR_3DPrinterFrontTopCentre_Temperature"
"SENSOR_3DPrinterFrontTopRight_Temperature"
"SENSOR_3DPrinterFrontLeft00_Temperature"
"SENSOR_3DPrinterFrontLeft25_Temperature"
"SENSOR_3DPrinterFrontLeft50_Temperature"
"SENSOR_3DPrinterFrontLeft75_Temperature"
"SENSOR_3DPrinterFrontLeft100_Temperature"
];

ref = "SENSOR_3DPrinterFrontRight25_Temperature";   % choose your reference

%% Part 1 - manual basic query and show response
one = meas(1);
if isfield(cfg,'rp') && ~isempty(cfg.rp)
    fromClause = sprintf('"%s"."%s"', cfg.rp, one);
else
    fromClause = sprintf('"%s"', one);
end
q = sprintf(['SELECT "value" FROM %s WHERE time >= ''%s'' AND time <= ''%s'' ORDER BY time ASC'], ...
             fromClause, ...
             datestr(datetime(tStart,'TimeZone','UTC'), 'yyyy-mm-ddTHH:MM:SSZ'), ...
             datestr(datetime(tEnd,  'TimeZone','UTC'), 'yyyy-mm-ddTHH:MM:SSZ'));
url = Influx_MakeQueryURL(cfg.baseURL, cfg.db, q, 'ms');
raw = webread(url, weboptions('Timeout',60,'ContentType','text'));
resp = jsondecode(raw);
disp('Columns:');  disp(resp.results(1).series(1).columns);
V = resp.results(1).series(1).values;
if isnumeric(V)
    fprintf('First 5 rows [ms, value]:\n'); disp(V(1:min(5,end),:));
else
    fprintf('First 3 rows (cell rows):\n'); disp(V(1:min(3,end),:));
end

%% Part 2 - query one value and plot
[Series1, ~] = SensorCalibrationQueryInflux(cfg, one, tStart, tEnd, 'ValueMin', -inf, 'ValueMax', +inf);
key1 = matlab.lang.makeValidName(one);
tt1  = Series1.(key1);
figure('Name','Single Sensor'); plot(tt1.t, tt1.value, '-o'); grid on;
title(sprintf('%s', one), 'Interpreter','none'); xlabel('UTC time'); ylabel('value');

%% Part 3 - query all and plot
%% Part 3 - query all and plot   % (Δ updated)
% [Series, All] = SensorCalibrationQueryInflux(cfg, meas, tStart, tEnd, 'ValueMin', 10, 'ValueMax', 80);
% 
% figure('Name','All Sensors (overlay)'); hold on; grid on;
% keys = string(fieldnames(Series));
% for i = 1:numel(keys)
%     tt = Series.(keys(i));
%     if isempty(tt) || height(tt)==0 || all(isnan(tt.value)), continue; end
%     rt = tt.Properties.RowTimes;                 % use each series' own timestamps
%     lbl = erase(erase(erase(keys(i),"SENSOR_"),"_Temperature"),"3DPrinter");
%     plot(rt, tt.value, '*', 'DisplayName', lbl);
% end
% legend('Location','bestoutside');
% title('All temps'); xlabel('UTC time'); ylabel('value');

%% Part 3 - query all and plot (60 s binning)
[SeriesRaw, ~] = SensorCalibrationQueryInflux(cfg, meas, tStart, tEnd, 'ValueMin',10,'ValueMax',80);
[Series60, All60] = BinSeriesMean(SeriesRaw, 60, tStart, tEnd);

figure('Name','All Sensors (60s means)'); hold on; grid on;
rt   = All60.Properties.RowTimes;           % unified timestamps
vars = All60.Properties.VariableNames;
for i = 1:numel(vars)
    y = All60.(vars{i});
    if all(isnan(y)), continue; end
    lbl = erase(erase(erase(vars{i},"SENSOR_"),"_Temperature"),"3DPrinter");
    plot(rt, y, 'DisplayName', lbl);
end
legend('Location','bestoutside');
title('All temps (60s averages)'); xlabel('UTC time'); ylabel('value');


%%
% after Part 3 where you have Series60 and All60
ref = "SENSOR_3DPrinterFrontRight25_Temperature";   % or whichever

profiles = ComputeSensorCalibration(Series60, ref, 1);  % 1=linear, 2=quadratic
ExportCalibrationProfiles(profiles, ref, 'calibration_sensor_profiles.json', ...
    'Units','degC', 'Notes','60s means');



%% Part 4 - query all, calibration, output data
%% Part 4 - query all, calibration, output data (on 60s grid)
profiles = ComputeSensorCalibration(Series60, ref, 1);   % 1=linear, 2=quadratic
ExportCalibrationProfiles(profiles, ref, 'calibration_sensor_profiles.json', ...
    'Units','degC', 'Notes','Hot-water cool-down test');

% Residuals vs reference on the same 60s grid
refKey = matlab.lang.makeValidName(ref);
ttRef  = Series60.(refKey); ttRef.Properties.VariableNames = {'y'};

figure('Name','Residuals vs ref (60s means)'); hold on; grid on;
for i = 1:numel(profiles)
    name = string(profiles(i).name);
    tt   = Series60.(name);
    if isempty(tt) || height(tt) < 2, continue; end

    % Apply calibration, then differ on the shared grid (no extra sync needed)
    ttCal  = ApplyCalibration(tt, profiles(i).model, profiles(i).coeffs);
    ttCalX = ttCal(:, 'value_cal'); ttCalX.Properties.VariableNames = {'yc'};

    rt    = ttRef.Properties.RowTimes;
    valid = ~isnan(ttCalX.yc) & ~isnan(ttRef.y);
    if any(valid)
        plot(rt(valid), ttCalX.yc(valid) - ttRef.y(valid), ...
             'DisplayName', erase(erase(erase(name,"SENSOR_"),"_Temperature"),"3DPrinter"));
    end
end
legend('Location','bestoutside');
title('Calibrated minus reference (60s means)'); ylabel('°C'); xlabel('UTC time');



%%

% After computing profiles and applying them to Series60
refKey = matlab.lang.makeValidName(ref);
ttRef  = Series60.(refKey); ttRef.Properties.VariableNames = {'y'};

biasTable = table('Size',[0 2], 'VariableTypes',["string","double"], ...
                  'VariableNames',{'Name','MeanResidual'});

for i = 1:numel(profiles)
    name = string(profiles(i).name);
    tt   = Series60.(name); if isempty(tt), continue; end
    ttCal  = ApplyCalibration(tt, profiles(i).model, profiles(i).coeffs);
    ttCalX = ttCal(:, 'value_cal'); ttCalX.Properties.VariableNames = {'yc'};
    valid = ~isnan(ttCalX.yc) & ~isnan(ttRef.y);
    bias  = mean(ttCalX.yc(valid) - ttRef.y(valid), 'omitnan');
    biasTable = [biasTable; {erase(erase(erase(name,"SENSOR_"),"_Temperature"),"3DPrinter"), bias}];
end
disp(biasTable);
