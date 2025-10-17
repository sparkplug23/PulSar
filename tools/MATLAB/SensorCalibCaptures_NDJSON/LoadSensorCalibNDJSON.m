function [T, BySensor, sensorNames, typeNames] = LoadSensorCalibNDJSON(fn)
% Date Modified: 04 Oct 2025
% SUMMARY: Load NDJSON → table T and grouped struct BySensor.(name).(type)=timetable

recs = i_read_ndjson_records(fn);
if isempty(recs)
    T = table(); BySensor = struct(); sensorNames = strings(0,1); typeNames = strings(0,1);
    return;
end

n   = numel(recs);
t   = zeros(n,1,'double');
nm  = strings(n,1);
tid = zeros(n,1,'uint16');
val = zeros(n,1,'double');

for k = 1:n
    rk   = recs{k};
    t(k) = double(rk.t);
    nm(k)= string(rk.name);
    tid(k)= uint16(rk.type_id);
    val(k)= double(rk.v);
end

timeUTC = datetime(t,'ConvertFrom','posixtime','TimeZone','UTC');

typ = strings(n,1);
for k = 1:n
    typ(k) = string(SensorTypeIdToName(tid(k)));
end

T = table(timeUTC, t, nm, tid, typ, val, ...
    'VariableNames', {'time','t','name','type_id','type','v'});
T = sortrows(T, {'name','type','time'});

[BySensor, sensorNames, typeNames] = i_group_by_sensor(T);

% ---------- helpers (local to this file) ----------
function S = i_read_ndjson_records(f)
    S = {};
    fid = fopen(f,'r'); if fid < 0, error('Cannot open %s', f); end
    c = onCleanup(@() fclose(fid));
    while true
        L = fgetl(fid); if ~ischar(L), break; end
        L = strtrim(L);
        if isempty(L), continue; end
        try
            obj = jsondecode(L);
            if isstruct(obj) && all(isfield(obj, {'t','name','type_id','v'}))
                S{end+1} = obj; %#ok<AGROW>
            end
        catch
            % skip malformed line
        end
    end
end

function [G, sensorNames, typeNames] = i_group_by_sensor(TT)
    G = struct();
    sensorNames = unique(TT.name, 'stable');
    typeNames   = unique(TT.type, 'stable');
    for i = 1:numel(sensorNames)
        sName  = sensorNames(i);
        Ti     = TT(TT.name==sName,:);
        sField = matlab.lang.makeValidName(char(sName));
        if ~isfield(G, sField), G.(sField) = struct(); end
        tNames = unique(Ti.type, 'stable');
        for j = 1:numel(tNames)
            tName  = tNames(j);
            Tj     = Ti(Ti.type==tName, ["time","v"]);
            tt     = timetable(Tj.time, Tj.v, 'VariableNames', {'v'});
            tField = matlab.lang.makeValidName(char(tName));
            G.(sField).(tField) = sortrows(tt,'Time');
        end
    end
end
end
