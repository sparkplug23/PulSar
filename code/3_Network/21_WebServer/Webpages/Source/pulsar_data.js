/*
 * PulSar Data Web UI
 *
 * Optional helpers for data-heavy PulSar pages.
 *
 * Requires:
 *   /pulsar.js
 *
 * Intended for:
 *   diagnostics
 *   telemetry
 *   Tasker metrics
 *   runtime/system information
 *   dynamic tables
 *   polling pages
 */


/**************************************************************************************************
 * Number / time formatting
 **************************************************************************************************/

function formatNumber(value, digits = 0)
{
  const number = Number(value);

  if(!Number.isFinite(number))
  {
    return '—';
  }

  return number.toLocaleString(undefined,{
    minimumFractionDigits:digits,
    maximumFractionDigits:digits
  });
}


function formatPercent(value, digits = 2)
{
  const number = Number(value);

  if(!Number.isFinite(number))
  {
    return '—';
  }

  return formatNumber(number,digits) + '%';
}


function formatBytes(bytes, digits = 1)
{
  let value = Number(bytes);

  if(!Number.isFinite(value))
  {
    return '—';
  }

  const units = ['B','KB','MB','GB','TB'];
  let unit = 0;

  while(Math.abs(value) >= 1024 && unit < units.length - 1)
  {
    value /= 1024;
    unit++;
  }

  return formatNumber(value,unit ? digits : 0) + ' ' + units[unit];
}


function formatDuration(ms)
{
  let value = Number(ms);

  if(!Number.isFinite(value) || value < 0)
  {
    return '—';
  }

  if(value < 1000)
  {
    return formatNumber(value,0) + ' ms';
  }

  if(value < 60000)
  {
    return formatNumber(value / 1000,1) + ' s';
  }

  if(value < 3600000)
  {
    return formatNumber(value / 60000,1) + ' min';
  }

  if(value < 86400000)
  {
    return formatNumber(value / 3600000,2) + ' hr';
  }

  return formatNumber(value / 86400000,2) + ' d';
}


function formatTimestamp(value)
{
  const date = value instanceof Date ? value : new Date(value);

  if(Number.isNaN(date.getTime()))
  {
    return '—';
  }

  return date.toLocaleString();
}


function formatTime(value)
{
  const date = value instanceof Date ? value : new Date(value);

  if(Number.isNaN(date.getTime()))
  {
    return '—';
  }

  return date.toLocaleTimeString();
}


/**************************************************************************************************
 * Fetch
 **************************************************************************************************/

async function fetchResponse(url, options = {})
{
  const config = Object.assign({
    cache:'no-store'
  },options);

  const response = await fetch(url,config);

  if(!response.ok)
  {
    let detail = '';

    try
    {
      detail = await response.text();
    }
    catch(e)
    {
    }

    const error = new Error(detail || ('HTTP ' + response.status));

    error.status = response.status;
    error.response = response;

    throw error;
  }

  return response;
}


async function getJSON(url, options = {})
{
  const response = await fetchResponse(url,options);
  return response.json();
}


async function getText(url, options = {})
{
  const response = await fetchResponse(url,options);
  return response.text();
}


async function postJSON(url, data, options = {})
{
  const config = Object.assign({},options,{
    method:'POST',
    headers:Object.assign({
      'Content-Type':'application/json'
    },options.headers || {}),
    body:JSON.stringify(data)
  });

  const response = await fetchResponse(url,config);

  if(response.status === 204)
  {
    return null;
  }

  return response.json();
}


/**************************************************************************************************
 * Polling
 **************************************************************************************************/

async function pollUntil(fn, options = {})
{
  const interval = Number(options.interval ?? 100);
  const attempts = Number(options.attempts ?? 50);

  for(let attempt = 0; attempt < attempts; attempt++)
  {
    const result = await fn(attempt);

    if(result)
    {
      return result;
    }

    if(attempt < attempts - 1)
    {
      await sleep(interval);
    }
  }

  return null;
}


function createPoller(callback, interval = 10000)
{
  let timer = 0;
  let running = false;

  async function run()
  {
    if(running)
    {
      return;
    }

    running = true;

    try
    {
      await callback();
    }
    finally
    {
      running = false;
    }
  }

  return {
    start(immediate = true)
    {
      this.stop();

      if(immediate)
      {
        run();
      }

      timer = setInterval(run,interval);
    },

    stop()
    {
      if(timer)
      {
        clearInterval(timer);
        timer = 0;
      }
    },

    refresh()
    {
      return run();
    },

    setInterval(ms)
    {
      interval = Number(ms) || 0;

      if(timer)
      {
        this.start(false);
      }
    },

    isRunning()
    {
      return running;
    }
  };
}


/**************************************************************************************************
 * Generic tables
 **************************************************************************************************/

function makeCell(value, className = '')
{
  const td = cE('td');

  if(className)
  {
    td.className = className;
  }

  td.textContent = value == null ? '' : value;

  return td;
}


function makeHTMLCell(html, className = '')
{
  const td = cE('td');

  if(className)
  {
    td.className = className;
  }

  td.innerHTML = html == null ? '' : html;

  return td;
}


function makeRow(values, options = {})
{
  const tr = cE('tr');

  if(options.className)
  {
    tr.className = options.className;
  }

  for(const value of values)
  {
    if(value instanceof Node)
    {
      const td = cE('td');
      td.appendChild(value);
      tr.appendChild(td);
    }
    else
    {
      tr.appendChild(makeCell(value));
    }
  }

  return tr;
}


function appendRow(body, values, options = {})
{
  if(typeof body === 'string')
  {
    body = gId(body);
  }

  if(!body)
  {
    return null;
  }

  const row = makeRow(values,options);

  body.appendChild(row);

  return row;
}


function clearTable(body)
{
  return clearElement(body);
}


function setTableEmpty(body, colspan, text = 'No data')
{
  if(typeof body === 'string')
  {
    body = gId(body);
  }

  if(!body)
  {
    return null;
  }

  clearElement(body);

  const tr = cE('tr');
  const td = cE('td');

  td.colSpan = colspan;
  td.className = 'empty';
  td.textContent = text;

  tr.appendChild(td);
  body.appendChild(tr);

  return tr;
}


/**************************************************************************************************
 * Key / value builders
 **************************************************************************************************/

function appendKVRow(container, key, value, options = {})
{
  if(typeof container === 'string')
  {
    container = gId(container);
  }

  if(!container)
  {
    return null;
  }

  const row = cE('div');
  row.className = 'kv-row';

  const keyElement = cE('div');
  keyElement.className = 'kv-key';
  keyElement.textContent = key == null ? '' : key;

  const valueElement = cE('div');
  valueElement.className = 'kv-value';

  if(options.html)
  {
    valueElement.innerHTML = value == null ? '' : value;
  }
  else
  {
    valueElement.textContent = value == null ? '' : value;
  }

  row.appendChild(keyElement);
  row.appendChild(valueElement);

  container.appendChild(row);

  return row;
}


/**************************************************************************************************
 * Generic object rendering
 **************************************************************************************************/

function flattenObject(source, prefix = '', output = {})
{
  if(!source || typeof source !== 'object' || Array.isArray(source))
  {
    return output;
  }

  for(const [key,value] of Object.entries(source))
  {
    const path = prefix ? prefix + '.' + key : key;

    if(value && typeof value === 'object' && !Array.isArray(value))
    {
      flattenObject(value,path,output);
    }
    else
    {
      output[path] = value;
    }
  }

  return output;
}


function renderObjectKV(container, data, options = {})
{
  if(typeof container === 'string')
  {
    container = gId(container);
  }

  if(!container)
  {
    return;
  }

  clearElement(container);

  const source = options.flatten === false ? data : flattenObject(data);

  for(const [key,value] of Object.entries(source || {}))
  {
    appendKVRow(container,key,formatDataValue(value,options));
  }
}


/**************************************************************************************************
 * Generic data formatting
 **************************************************************************************************/

function formatDataValue(value, options = {})
{
  if(value === null || value === undefined)
  {
    return options.empty ?? '—';
  }

  if(typeof value === 'boolean')
  {
    return value ? 'Yes' : 'No';
  }

  if(typeof value === 'number')
  {
    return formatNumber(value,options.digits ?? (Number.isInteger(value) ? 0 : 3));
  }

  if(Array.isArray(value))
  {
    return value.join(', ');
  }

  if(typeof value === 'object')
  {
    try
    {
      return JSON.stringify(value);
    }
    catch(e)
    {
      return String(value);
    }
  }

  return String(value);
}


/**************************************************************************************************
 * Forms
 **************************************************************************************************/

function formToObject(form)
{
  if(typeof form === 'string')
  {
    form = gId(form);
  }

  if(!form)
  {
    return {};
  }

  const result = {};
  const data = new FormData(form);

  for(const [key,value] of data.entries())
  {
    if(Object.prototype.hasOwnProperty.call(result,key))
    {
      if(!Array.isArray(result[key]))
      {
        result[key] = [result[key]];
      }

      result[key].push(value);
    }
    else
    {
      result[key] = value;
    }
  }

  for(const input of form.querySelectorAll('input[type="checkbox"][name]'))
  {
    result[input.name] = input.checked;
  }

  return result;
}