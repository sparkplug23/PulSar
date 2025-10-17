function Summary = SummarizeProfiles(profiles)
names = strings(numel(profiles),1);
model = strings(numel(profiles),1);
rmse  = zeros(numel(profiles),1);
npts  = zeros(numel(profiles),1);
rangeLo = zeros(numel(profiles),1);
rangeHi = zeros(numel(profiles),1);
A = nan(numel(profiles),1); B = nan(numel(profiles),1);
A2 = nan(numel(profiles),1); % for quadratic

for i=1:numel(profiles)
    p = profiles(i);
    names(i) = string(p.name);
    model(i) = string(p.model);
    rmse(i)  = p.rmse;
    npts(i)  = p.n;
    rangeLo(i) = p.range(1); rangeHi(i) = p.range(2);
    if p.model == "poly1"
        A(i) = p.coeffs.a;  B(i) = p.coeffs.b;
    else
        A2(i)= p.coeffs.a2; A(i)= p.coeffs.a1; B(i)= p.coeffs.a0;
    end
end

Summary = table(names, model, A2, A, B, rmse, npts, rangeLo, rangeHi, ...
    'VariableNames', {'Name','Model','a2','a1_or_a','b','RMSE','N','InLo','InHi'});
Summary = sortrows(Summary, 'RMSE');
disp(Summary);
end

% usage:
Summary = SummarizeProfiles(profiles);
