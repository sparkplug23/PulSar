function [SeriesBin, AllBin] = BinSeriesMean(Series, binSeconds, tStart, tEnd)
% Average each Series.<name> over fixed time bins, returning a unified grid.
% - Series: struct of timetables with variable 'value'
% - binSeconds: scalar (e.g., 60) or duration (seconds(60))
% - tStart/tEnd: datetimes (any TZ; we use UTC internally)

% normalize step + window
if isa(binSeconds,'duration'), step = binSeconds; else, step = seconds(binSeconds); end
t0 = datetime(tStart,'TimeZone','UTC');
t1 = datetime(tEnd,  'TimeZone','UTC');

% anchor to clean boundaries (minute if step >= 60s)
if seconds(step) >= 60
    g0 = dateshift(t0,'start','minute');
    g1 = dateshift(t1,'end','minute');
else
    g0 = dateshift(t0,'start','second');
    g1 = t1;
end
grid = (g0:step:g1).';

SeriesBin = struct();
ttList = {};
keys = string(fieldnames(Series));

for i = 1:numel(keys)
    k  = keys(i);
    tt = Series.(k);
    if isempty(tt)
        SeriesBin.(k) = tt;    %#ok<AGROW>
        continue
    end
    % bin by mean onto the unified grid
    ttB = retime(tt, grid, 'mean');
    SeriesBin.(k) = ttB;       %#ok<AGROW>

    % for combined table, give each its own column name
    ttx = timetable(ttB.Properties.RowTimes, ttB.value, 'VariableNames', {char(k)});
    ttList{end+1} = ttx;       %#ok<AGROW>
end

% outer-join across all (they all share the same grid)
if isempty(ttList)
    AllBin = timetable();
else
    AllBin = ttList{1};
    for i = 2:numel(ttList)
        AllBin = synchronize(AllBin, ttList{i});
    end
end
end
