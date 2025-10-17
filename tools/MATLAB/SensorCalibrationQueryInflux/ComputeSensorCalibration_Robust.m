function profiles = ComputeSensorCalibration_Robust(Series, refMeasurement, modelOrder, trimPct)
% trimPct: e.g., 5 means drop top 5% |x - median(x)| and |y - median(y)|
if nargin<4, trimPct = 5; end

refKey = matlab.lang.makeValidName(string(refMeasurement));
ttRef  = Series.(refKey);
ttR = ttRef; ttR.Properties.VariableNames = {'y'};

profiles = struct('name',{},'model',{},'coeffs',{},'rmse',{},'n',{},'range',{});
keys = string(fieldnames(Series));
for i = 1:numel(keys)
    k = keys(i); if k == string(refKey), continue; end
    tt = Series.(k); if isempty(tt) || height(tt)<2, continue; end

    % align to reference grid
    tx = retime(tt, ttR.Properties.RowTimes, 'linear');
    tx.Properties.VariableNames = {'x'};
    T  = synchronize(tx, ttR, 'intersection');
    T  = rmmissing(T);
    if height(T) < 10, continue; end
    x = T.x; y = T.y;

    % trim outliers symmetrically on both axes
    ax = abs(x - median(x,'omitnan'));
    ay = abs(y - median(y,'omitnan'));
    kx = ax <= prctile(ax, 100-trimPct);
    ky = ay <= prctile(ay, 100-trimPct);
    keep = kx & ky;
    x = x(keep); y = y(keep);
    if numel(x) < 10, continue; end

    % fit
    if modelOrder==1
        p = polyfit(x,y,1); yhat = polyval(p,x);
        model='poly1'; coeffs=struct('a',p(1),'b',p(2));
    else
        p = polyfit(x,y,2); yhat = polyval(p,x);
        model='poly2'; coeffs=struct('a2',p(1),'a1',p(2),'a0',p(3));
    end

    rmse = sqrt(mean((y-yhat).^2));
    profiles(end+1) = struct('name',string(k),'model',model,'coeffs',coeffs, ...
                             'rmse',rmse,'n',numel(x),'range',[min(x) max(x)]);
end
end

% usage:
profiles = ComputeSensorCalibration_Robust(Series60, ref, 1, 5);  % 5% trim
