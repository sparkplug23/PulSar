function formatNumber(v,digits=0){
  const n=Number(v);
  return Number.isFinite(n)?n.toLocaleString(undefined,{minimumFractionDigits:digits,maximumFractionDigits:digits}):'—';
}

function formatPercent(v,d=2){return formatNumber(v,d)+'%';}

function formatBytes(v,d=1){
  let n=Number(v);
  if(!Number.isFinite(n))return '—';
  const u=['B','KB','MB','GB','TB'];
  let i=0;
  while(Math.abs(n)>=1024&&i<u.length-1){n/=1024;i++;}
  return formatNumber(n,i?d:0)+' '+u[i];
}

function formatDuration(ms){
  const n=Number(ms);
  if(!Number.isFinite(n)||n<0)return '—';
  if(n<1000)return formatNumber(n)+' ms';
  if(n<60000)return formatNumber(n/1000,1)+' s';
  if(n<3600000)return formatNumber(n/60000,1)+' min';
  if(n<86400000)return formatNumber(n/3600000,2)+' hr';
  return formatNumber(n/86400000,2)+' d';
}

function formatTime(v){
  const x=new Date(v);
  return Number.isNaN(x.getTime())?'—':x.toLocaleTimeString();
}

async function fetchResponse(url,opt={}){
  const r=await fetch(url,Object.assign({cache:'no-store'},opt));
  if(!r.ok){
    let t='';
    try{t=await r.text();}catch(e){}
    const x=new Error(t||'HTTP '+r.status);
    x.status=r.status;
    x.response=r;
    throw x;
  }
  return r;
}

async function getJSON(url,opt={}){return (await fetchResponse(url,opt)).json();}
async function getText(url,opt={}){return (await fetchResponse(url,opt)).text();}

async function postJSON(url,data,opt={}){
  const r=await fetchResponse(url,Object.assign({},opt,{
    method:'POST',
    headers:Object.assign({'Content-Type':'application/json'},opt.headers||{}),
    body:JSON.stringify(data)
  }));
  return r.status===204?null:r.json();
}

function createPoller(fn,interval=10000){
  let timer=0,running=false;

  async function run(){
    if(running)return;
    running=true;
    try{await fn();}
    finally{running=false;}
  }

  return {
    start(immediate=true){
      this.stop();
      if(immediate)run();
      if(interval)timer=setInterval(run,interval);
    },
    stop(){
      if(timer){clearInterval(timer);timer=0;}
    },
    refresh:run,
    setInterval(ms){
      interval=Number(ms)||0;
      if(timer)this.start(false);
    }
  };
}

function makeCell(v,cls=''){
  const td=cE('td');
  if(cls)td.className=cls;
  td.textContent=v==null?'':v;
  return td;
}

function appendRow(body,values,cls=''){
  body=el(body);
  if(!body)return;
  const tr=cE('tr');
  if(cls)tr.className=cls;
  for(const v of values)tr.appendChild(makeCell(v));
  body.appendChild(tr);
  return tr;
}

function setTableEmpty(body,cols,text='No data'){
  body=clearElement(body);
  if(!body)return;
  const tr=cE('tr'),td=makeCell(text,'empty');
  td.colSpan=cols;
  tr.appendChild(td);
  body.appendChild(tr);
}

function formatDataValue(v,digits=3){
  if(v==null)return '—';
  if(typeof v==='boolean')return v?'Yes':'No';
  if(typeof v==='number')return formatNumber(v,Number.isInteger(v)?0:digits);
  if(Array.isArray(v))return v.join(', ');
  if(typeof v==='object'){
    try{return JSON.stringify(v);}
    catch(e){}
  }
  return String(v);
}

function createWebSocketClient(path,onMessage,onOpen,retry=2000){
  let ws=null,timer=0,closed=false;

  function connect(){
    if(closed||ws&&(ws.readyState===0||ws.readyState===1))return;

    ws=new WebSocket((location.protocol==='https:'?'wss://':'ws://')+location.host+path);

    ws.onopen=()=>{if(onOpen)onOpen(client);};

    ws.onmessage=e=>{
      let data;
      try{data=JSON.parse(e.data);}
      catch(x){return;}
      if(onMessage)onMessage(data,client);
    };

    ws.onclose=()=>{
      ws=null;
      if(!closed&&!timer)timer=setTimeout(()=>{timer=0;connect();},retry);
    };

    ws.onerror=()=>{if(ws)ws.close();};
  }

  const client={
    connect,
    send(data){
      if(!ws||ws.readyState!==1)return false;
      ws.send(typeof data==='string'?data:JSON.stringify(data));
      return true;
    },
    close(){
      closed=true;
      if(timer)clearTimeout(timer);
      if(ws)ws.close();
    }
  };

  return client;
}