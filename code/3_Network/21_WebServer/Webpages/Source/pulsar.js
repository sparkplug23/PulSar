'use strict';

const d=document;

function gId(id){return d.getElementById(id);}
function cE(tag){return d.createElement(tag);}
function qS(s,p=d){return p.querySelector(s);}
function qSA(s,p=d){return p.querySelectorAll(s);}

function el(v){return typeof v==='string'?gId(v):v;}

function setText(id,v){
  const e=el(id);
  if(e)e.textContent=v==null?'':v;
  return e;
}

function setHTML(id,v){
  const e=el(id);
  if(e)e.innerHTML=v==null?'':v;
  return e;
}

function show(id,on=true){
  const e=el(id);
  if(e)e.classList.toggle('hide',!on);
  return e;
}

function clearElement(id){
  const e=el(id);
  if(e)e.replaceChildren();
  return e;
}

function go(path){
  if(!path)return;
  if(!/^https?:\/\//i.test(path)&&path[0]!=='/')path='/'+path;
  location.href=path;
}

function back(){history.back();}

function getURL(path='/'){
  if(path[0]!=='/')path='/'+path;
  return location.origin+path;
}

function esc(v){
  return String(v??'')
    .replaceAll('&','&amp;')
    .replaceAll('<','&lt;')
    .replaceAll('>','&gt;')
    .replaceAll('"','&quot;')
    .replaceAll("'",'&#039;');
}

function safeId(v){return String(v??'').replace(/[^a-zA-Z0-9_-]/g,'_');}

function setStatus(id,text,state=''){
  const e=el(id);
  if(!e)return;
  e.textContent=text??'';
  e.classList.remove('ok','warn','bad');
  if(state)e.classList.add(state);
  return e;
}

let toastTimer=0;
function showToast(text,error=false,timeout=2900){
  const e=gId('toast');
  if(!e)return;
  e.textContent=text;
  e.className=error?'error':'show';
  clearTimeout(toastTimer);
  toastTimer=setTimeout(()=>e.className='',timeout);
}

function makeButton(text,fn,cls=''){
  const b=cE('button');
  b.type='button';
  b.textContent=text;
  if(cls)b.className=cls;
  if(fn)b.onclick=fn;
  return b;
}

function makeTile(title,desc='',url='',showUrl=false){
  const t=cE('div');
  t.className='tile menu-tile clickable';
  t.onclick=()=>go(url);

  t.innerHTML=
    '<div class="tile-title"></div>'+
    '<div class="tile-description"></div>'+
    (showUrl&&url?'<div class="tile-url"></div>':'');

  setText(t.children[0],title);
  setText(t.children[1],desc);

  if(showUrl&&url)
  {
    setText(t.children[2],url);
  }

  return t;
}

function renderMenu(id,items){
  const box=clearElement(id);
  if(!box)return;
  for(const i of items)box.appendChild(makeTile(i[0],i[1]||'',i[2]||''));
}

function sleep(ms){return new Promise(r=>setTimeout(r,ms));}

function setOriginText(id='origin'){setText(id,location.origin||'—');}

function onReady(fn){
  if(d.readyState==='loading')d.addEventListener('DOMContentLoaded',fn,{once:true});
  else fn();
}