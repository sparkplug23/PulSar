/*
 * PulSar Core Web UI
 *
 * Lightweight helpers intended for all native PulSar pages.
 */

'use strict';

const d = document;


/**************************************************************************************************
 * DOM
 **************************************************************************************************/

function gId(id)
{
  return d.getElementById(id);
}


function cE(tag)
{
  return d.createElement(tag);
}


function qS(selector, parent = d)
{
  return parent.querySelector(selector);
}


function qSA(selector, parent = d)
{
  return parent.querySelectorAll(selector);
}


function setText(id, value)
{
  const el = typeof id === 'string' ? gId(id) : id;

  if(el)
  {
    el.textContent = value == null ? '' : value;
  }

  return el;
}


function setHTML(id, value)
{
  const el = typeof id === 'string' ? gId(id) : id;

  if(el)
  {
    el.innerHTML = value == null ? '' : value;
  }

  return el;
}


function setVisible(id, visible)
{
  const el = typeof id === 'string' ? gId(id) : id;

  if(el)
  {
    el.classList.toggle('hide', !visible);
  }

  return el;
}


function clearElement(id)
{
  const el = typeof id === 'string' ? gId(id) : id;

  if(!el)
  {
    return null;
  }

  while(el.firstChild)
  {
    el.removeChild(el.firstChild);
  }

  return el;
}


/**************************************************************************************************
 * Navigation
 **************************************************************************************************/

function go(path)
{
  if(!path)
  {
    return;
  }

  if(/^https?:\/\//i.test(path))
  {
    location.href = path;
    return;
  }

  if(path[0] !== '/')
  {
    path = '/' + path;
  }

  location.href = path;
}


function back()
{
  history.back();
}


function getURL(path)
{
  path = path || '/';

  if(path[0] !== '/')
  {
    path = '/' + path;
  }

  return location.origin + path;
}


/**************************************************************************************************
 * Strings / IDs
 **************************************************************************************************/

function esc(value)
{
  return String(value ?? '')
    .replaceAll('&','&amp;')
    .replaceAll('<','&lt;')
    .replaceAll('>','&gt;')
    .replaceAll('"','&quot;')
    .replaceAll("'",'&#039;');
}


function safeId(value)
{
  return String(value ?? '').replace(/[^a-zA-Z0-9_-]/g,'_');
}


/**************************************************************************************************
 * Status
 **************************************************************************************************/

function setStatus(id, text, state = '')
{
  const el = typeof id === 'string' ? gId(id) : id;

  if(!el)
  {
    return null;
  }

  el.textContent = text == null ? '' : text;
  el.classList.remove('ok','warn','bad');

  if(state === 'ok' || state === 'warn' || state === 'bad')
  {
    el.classList.add(state);
  }

  return el;
}


/**************************************************************************************************
 * Toast
 **************************************************************************************************/

let pulsarToastTimer = 0;


function showToast(text, error = false, timeout = 2900)
{
  const el = gId('toast');

  if(!el)
  {
    return;
  }

  el.textContent = text;
  el.className = error ? 'error' : 'show';

  clearTimeout(pulsarToastTimer);

  pulsarToastTimer = setTimeout(function()
  {
    el.className = '';
  },timeout);
}


/**************************************************************************************************
 * Generic element builders
 **************************************************************************************************/

function makeButton(text, onclick, options = {})
{
  const button = cE('button');

  button.type = options.type || 'button';
  button.textContent = text == null ? '' : text;

  if(options.className)
  {
    button.className = options.className;
  }

  if(options.title)
  {
    button.title = options.title;
  }

  if(onclick)
  {
    button.onclick = onclick;
  }

  return button;
}


function makeTile(title, description = '', options = {})
{
  const tile = cE('div');

  tile.className = 'tile';

  const titleElement = cE('div');
  titleElement.className = 'tile-title';
  titleElement.textContent = title;

  tile.appendChild(titleElement);

  if(description)
  {
    const descriptionElement = cE('div');
    descriptionElement.className = 'tile-description';
    descriptionElement.textContent = description;
    tile.appendChild(descriptionElement);
  }

  if(options.url)
  {
    const urlElement = cE('div');
    urlElement.className = 'tile-url';
    urlElement.textContent = options.url;
    tile.appendChild(urlElement);
  }

  if(options.onclick)
  {
    tile.classList.add('clickable');
    tile.onclick = options.onclick;
  }

  return tile;
}


/**************************************************************************************************
 * Timing
 **************************************************************************************************/

function sleep(ms)
{
  return new Promise(resolve => setTimeout(resolve,ms));
}


/**************************************************************************************************
 * Misc
 **************************************************************************************************/

function setDocumentTitle(title)
{
  if(title)
  {
    document.title = title;
  }
}


function setOriginText(id = 'origin')
{
  setText(id,location.origin || '—');
}