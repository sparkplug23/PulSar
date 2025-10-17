function ttCal = ApplyCalibration(ttMeasured, model, coeffs)
% Apply y = f(x) to a timetable with variable 'value'
x = ttMeasured.value;
switch model
    case 'poly1', y = coeffs.a .* x + coeffs.b;
    case 'poly2', y = coeffs.a2 .* (x.^2) + coeffs.a1 .* x + coeffs.a0;
    otherwise,    error('Unknown model: %s', model);
end
ttCal = ttMeasured;
ttCal.value_cal = y;
end
