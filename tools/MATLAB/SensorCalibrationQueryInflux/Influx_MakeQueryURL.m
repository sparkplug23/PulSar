function url = Influx_MakeQueryURL(baseURL, db, q, epoch)
if ~endsWith(baseURL,'/'), baseURL = [baseURL '/']; end
base = [baseURL 'query'];
qs = ['db=' urlencode(db) '&epoch=' urlencode(epoch) '&q=' urlencode(q)];
url = [base '?' qs];
end
