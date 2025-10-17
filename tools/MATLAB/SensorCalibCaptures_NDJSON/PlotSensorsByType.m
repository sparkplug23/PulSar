function PlotSensorsByType(BySensor, typeName, sensors)
% Date Modified: 04 Oct 2025
% SUMMARY: Plot all (or selected) sensors for a given type vs time.
% ARGs:
%   BySensor : struct from LoadSensorCalibNDJSON
%   typeName : e.g. 'Temperature'
%   sensors  : optional string/cellstr filter of sensor names
fnType = matlab.lang.makeValidName(typeName);
allSensors = string(fieldnames(BySensor));
if nargin >= 3 && ~isempty(sensors)
    want = string(sensors);
    % map to valid fieldnames
    want = string(arrayfun(@(s) matlab.lang.makeValidName(char(s)), want,'uni',0));
    allSensors = intersect(allSensors, want, 'stable');
end
figure('Name',sprintf('%s — all sensors',typeName));
hold on;
shown = strings(0,1);
for i = 1:numel(allSensors)
    sField = allSensors(i);
    S = BySensor.(sField);
    if isfield(S, fnType)
        tt = S.(fnType);
        if ~isempty(tt)
            plot(tt.Time, tt.v, 'DisplayName', strrep(char(sField),'_','\_'));
            shown(end+1,1) = sField; %#ok<AGROW>
        end
    end
end
grid on; xlabel('Time (UTC)'); ylabel(typeName);
title(sprintf('%s vs time (%d)', typeName, numel(shown)));
if ~isempty(shown), legend('Location','bestoutside'); end
hold off;
end
