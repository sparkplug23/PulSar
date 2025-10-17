function [Series, AllTT] = SensorCalibrationQueryInflux(cfg, measurements, tStart, tEnd, varargin)
% Minimal InfluxDB 1.x (InfluxQL) loader for openHAB data.
% Assumes results(1).series(1).values has: [ epoch_ms , value ].
%
% cfg.baseURL, cfg.db, cfg.rp (e.g. 'autogen'), optional cfg.username/password
% cfg.stepSeconds: [] to skip resample; otherwise resample to fixed grid
% measurements: string/cell array of measurement names
% tStart/tEnd: datetimes (any TZ; converted to UTC)
% Name/Value: 'ValueMin','ValueMax' numeric filter

% options
p = inputParser;
p.addParameter('ValueMin', -inf, @isnumeric);
p.addParameter('ValueMax', +inf, @isnumeric);
p.parse(varargin{:});
vmin = p.Results.ValueMin; vmax = p.Results.ValueMax;

measurements = string(measurements(:));
if ~isfield(cfg,'stepSeconds'), cfg.stepSeconds = []; end   % no resample by default

% UTC window
tStartZ = datestr(datetime(tStart,'TimeZone','UTC'), 'yyyy-mm-ddTHH:MM:SSZ');
tEndZ   = datestr(datetime(tEnd,  'TimeZone','UTC'), 'yyyy-mm-ddTHH:MM:SSZ');

Series = struct();
ttList = {};

opts = weboptions('Timeout', 60, 'ContentType', 'text'); % always text → jsondecode
if isfield(cfg,'username') && ~isempty(cfg.username)
    opts.Username = cfg.username; opts.Password = cfg.password;
end

for k = 1:numel(measurements)
    meas = measurements(k);

    % FROM clause with RP if provided
    if isfield(cfg,'rp') && ~isempty(cfg.rp)
        fromClause = sprintf('"%s"."%s"', cfg.rp, meas);
    else
        fromClause = sprintf('"%s"', meas);
    end

    % RAW query (ascending time). No GROUP BY.
    q = sprintf(['SELECT "value" FROM %s ' ...
                 'WHERE time >= ''%s'' AND time <= ''%s'' ' ...
                 'ORDER BY time ASC'], ...
                 fromClause, tStartZ, tEndZ);

    url  = Influx_MakeQueryURL(cfg.baseURL, cfg.db, q, 'ms');
    raw  = webread(url, opts);
    resp = jsondecode(raw);

    % parse FIRST series → V(:,1)=ms, V(:,2)=value
    tt = timetable();
    if isstruct(resp) && isfield(resp,'results') && ~isempty(resp.results)
        R = resp.results; if iscell(R), R = R{1}; else, R = R(1); end
        if isfield(R,'series') && ~isempty(R.series)
            S = R.series;   if iscell(S), S = S{1}; else, S = S(1); end
            V = S.values;

            if isnumeric(V) && size(V,2) >= 2
                tms = double(V(:,1));
                val = double(V(:,2));
            elseif iscell(V) && ~isempty(V)
                n = numel(V); tms = zeros(n,1); val = zeros(n,1);
                if iscell(V{1})
                    for i = 1:n, row = V{i}; tms(i)=double(row{1}); val(i)=double(row{2}); end
                else
                    tms = cellfun(@double, V(:,1));
                    val = cellfun(@double, V(:,2));
                end
            else
                tms = []; val = [];
            end

            if ~isempty(tms)
                % epoch ms → datetime UTC
                t = datetime(tms/1000, 'ConvertFrom','posixtime', 'TimeZone','UTC');

                % optional numeric filter
                if ~isinf(vmin) || ~isinf(vmax)
                    keep = (val >= vmin) & (val <= vmax); t = t(keep); val = val(keep);
                end

                % timetable (var is always 'value'); row times are t
                tt = timetable(t, val, 'VariableNames', {'value'});

                % optional resample (use local t vector — no field-name assumptions)
                if ~isempty(cfg.stepSeconds) && numel(t) >= 2
                    grid = (t(1):seconds(cfg.stepSeconds):t(end)).';
                    tt   = retime(tt, grid, 'mean');
                end
            end
        end
    end

    % Store for calibration (var name stays 'value')
    key = matlab.lang.makeValidName(meas);
    Series.(key) = tt;

    % Also prepare for overlay plot / AllTT: give each series its own column name
    if ~isempty(tt)
        rt  = tt.Properties.RowTimes;           % row-time vector
        ttx = timetable(rt, tt.value, 'VariableNames', {char(key)});
        ttList{end+1} = ttx; %#ok<AGROW>
        fprintf('✓ %s: %d points\n', meas, height(tt));
    else
        fprintf('⚠ %s: 0 points in [%s .. %s]\n', meas, tStartZ, tEndZ);
    end
end

% synchronize all series (outer join on time)
if isempty(ttList)
    AllTT = timetable();
else
    AllTT = ttList{1};
    for i = 2:numel(ttList)
        AllTT = synchronize(AllTT, ttList{i});
    end
end
end
