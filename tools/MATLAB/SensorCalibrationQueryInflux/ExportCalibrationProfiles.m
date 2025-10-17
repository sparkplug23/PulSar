function ExportCalibrationProfiles(profiles, refMeasurement, filePath, varargin)
% Write calibration_sensor_profiles.json

p = inputParser;
p.addParameter('CreatedBy','SensorCalibrationQueryInflux', @ischar);
p.addParameter('Units','degC', @ischar);
p.addParameter('Notes','', @ischar);
p.parse(varargin{:});

if nargin < 3 || isempty(filePath)
    filePath = 'calibration_sensor_profiles.json';
end

meta.created_utc = char(datetime('now','TimeZone','UTC','Format','yyyy-MM-dd''T''HH:mm:ss''Z'''));
meta.reference   = string(refMeasurement);
meta.units       = p.Results.Units;
meta.created_by  = p.Results.CreatedBy;
meta.notes       = p.Results.Notes;

bundle = struct();
bundle.meta = meta;

if isempty(profiles)
    bundle.model    = "none";
    bundle.profiles = [];
else
    bundle.model = profiles(1).model;
    for i = 1:numel(profiles)
        profiles(i).alias = i_alias(profiles(i).name);
    end
    bundle.profiles = arrayfun(@(r) struct( ...
        'name', string(r.name), ...
        'alias', string(r.alias), ...
        'coeffs', r.coeffs, ...
        'rmse', r.rmse, ...
        'n', r.n, ...
        'input_range', r.range), profiles);
end

txt = jsonencode(bundle, 'PrettyPrint', true);
fid = fopen(filePath,'w'); assert(fid>0, 'Cannot open %s', filePath);
fwrite(fid, txt); fclose(fid);
fprintf('Wrote %s (%d profiles)\n', filePath, numel(bundle.profiles));
end

function a = i_alias(m)
% SENSOR_3DPrinterFrontRight25_Temperature -> FrontRight25
s = string(m);
s = erase(s,"SENSOR_");
s = erase(s,"_Temperature");
s = erase(s,"3DPrinter");
a = s;
end
