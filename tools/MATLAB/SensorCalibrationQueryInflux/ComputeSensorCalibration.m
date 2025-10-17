function profiles = ComputeSensorCalibration(Series, refMeasurement, modelOrder)
% Fit y_true = f(y_meas) per sensor vs reference.
% modelOrder: 1 (linear) or 2 (quadratic)

arguments
    Series struct
    refMeasurement {mustBeTextScalar}
    modelOrder (1,1) {mustBeMember(modelOrder,[1 2])} = 1
end

refKey = matlab.lang.makeValidName(string(refMeasurement));
if ~isfield(Series, refKey)
    error('Reference measurement "%s" not in Series.', refMeasurement);
end
ttRef = Series.(refKey);
if isempty(ttRef) || height(ttRef)<2
    error('Reference "%s" has no data.', refMeasurement);
end

% standardize names for sync
ttR = ttRef; ttR.Properties.VariableNames = {'y'};

profiles = struct('name',{},'model',{},'coeffs',{},'rmse',{},'n',{},'range',{});
keys = string(fieldnames(Series));

for i = 1:numel(keys)
    k = keys(i);
    if k == string(refKey), continue; end
    tt = Series.(k);
    if isempty(tt) || height(tt) < 2, continue; end

    tx = retime(tt, ttR.Properties.RowTimes, 'linear');   % align to ref timebase
tx.Properties.VariableNames = {'x'};
T  = synchronize(tx, ttR, 'intersection');
T  = rmmissing(T);                                    % drop NaNs after interp


    if height(T) < 10, continue; end

    x = T.x; y = T.y;

    if modelOrder == 1
        p = polyfit(x, y, 1);  yhat = polyval(p, x);
        model = 'poly1'; coeffs = struct('a', p(1), 'b', p(2));
    else
        p = polyfit(x, y, 2);  yhat = polyval(p, x);
        model = 'poly2'; coeffs = struct('a2', p(1), 'a1', p(2), 'a0', p(3));
    end

    rmse = sqrt(mean((y - yhat).^2));
    pr.name   = string(k);
    pr.model  = model;
    pr.coeffs = coeffs;
    pr.rmse   = rmse;
    pr.n      = numel(x);
    pr.range  = [min(x) max(x)];
    profiles(end+1) = pr; %#ok<AGROW>
end
end
