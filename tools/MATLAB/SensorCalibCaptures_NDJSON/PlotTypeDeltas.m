function PlotTypeDeltas(BySensor, typeName, refSensorName)
% Date Modified: 04 Oct 2025
% SUMMARY: Plot (sensor - reference) for all sensors of given type.
fnType = matlab.lang.makeValidName(typeName);
refField = matlab.lang.makeValidName(refSensorName);
assert(isfield(BySensor, refField) && isfield(BySensor.(refField), fnType), ...
    'Reference %s.%s not found', refSensorName, typeName);
ttRef = BySensor.(refField).(fnType);

figure('Name',sprintf('%s deltas vs %s',typeName,refSensorName)); hold on;
allSensors = string(fieldnames(BySensor));
for i = 1:numel(allSensors)
    sField = allSensors(i);
    if sField == string(refField), continue; end
    S = BySensor.(sField);
    if ~isfield(S, fnType), continue; end
    tt = S.(fnType);
    if isempty(tt), continue; end
    % synchronize to common time base (inner join)
    TT = synchronize(tt, ttRef, 'intersection');
    if height(TT)==0, continue; end
    d = TT.v_tt - TT.v_ttRef;
    plot(TT.Time, d, 'DisplayName', strrep(char(sField),'_','\_'));
end
grid on; xlabel('Time (UTC)'); ylabel(sprintf('%s - %s', typeName, refSensorName));
title(sprintf('%s deltas vs %s', typeName, refSensorName));
legend('Location','bestoutside'); hold off;
end
