var d=document;
var loc = false, locip, locproto = "http:";

function H(pg="")   { window.open("https://kno.wled.ge/"+pg); }
function GH()       { window.open("https://github.com/Aircoookie/WLED"); }
function gId(c)     { return d.getElementById(c); } // getElementById
function cE(e)      { return d.createElement(e); } // createElement
function gEBCN(c)   { return d.getElementsByClassName(c); } // getElementsByClassName
function gN(s)      { return d.getElementsByName(s)[0]; } // getElementsByName
function isE(o)     { return Object.keys(o).length === 0; } // isEmpty
function isO(i)     { return (i && typeof i === 'object' && !Array.isArray(i)); } // isObject
function isN(n)     { return !isNaN(parseFloat(n)) && isFinite(n); } // isNumber
// https://stackoverflow.com/questions/3885817/how-do-i-check-that-a-number-is-float-or-integer
function isF(n)     { return n === +n && n !== (n|0); } // isFloat
function isI(n)     { return n === +n && n === (n|0); } // isInteger
// function toggle(el) { gId(el).classList.toggle("hide"); gId('No'+el).classList.toggle("hide"); }
function toggle(el) {
	gId(el).classList.toggle("hide");
	let n = gId('No'+el);
	if (n) n.classList.toggle("hide");
}
function tooltip(cont=null) {
	d.querySelectorAll((cont?cont+" ":"")+"[title]").forEach((element)=>{
		element.addEventListener("mouseover", ()=>{
			// save title
			element.setAttribute("data-title", element.getAttribute("title"));
			const tooltip = d.createElement("span");
			tooltip.className = "tooltip";
			tooltip.textContent = element.getAttribute("title");

			// prevent default title popup
			element.removeAttribute("title");

			let { top, left, width } = element.getBoundingClientRect();

			d.body.appendChild(tooltip);

			const { offsetHeight, offsetWidth } = tooltip;

			const offset = element.classList.contains("sliderwrap") ? 4 : 10;
			top -= offsetHeight + offset;
			left += (width - offsetWidth) / 2;

			tooltip.style.top = top + "px";
			tooltip.style.left = left + "px";
			tooltip.classList.add("visible");
		});

		element.addEventListener("mouseout", ()=>{
			d.querySelectorAll('.tooltip').forEach((tooltip)=>{
				tooltip.classList.remove("visible");
				d.body.removeChild(tooltip);
			});
			// restore title
			element.setAttribute("title", element.getAttribute("data-title"));
		});
	});
};
// sequential loading of external resources (JS or CSS) with retry, calls init() when done
function loadResources(files, init) {
	let i = 0;
	const loadNext = () => {
		if (i >= files.length) {
			if (init) {
				d.documentElement.style.visibility = 'visible';
				d.readyState === 'complete' ? init() : window.addEventListener('load', init);
			}
			return;
		}

		const file = files[i++];
		const isCSS = file.endsWith('.css');
		const el = d.createElement(isCSS ? 'link' : 'script');

		if (isCSS) {
			el.rel = 'stylesheet';
			el.href = file;
			const st = d.head.querySelector('style');
			if (st) d.head.insertBefore(el, st);
			else d.head.appendChild(el);
		} else {
			el.src = file;
			d.head.appendChild(el);
		}

		el.onload = () => loadNext();
		el.onerror = () => {
			i--;
			setTimeout(loadNext, 100);
		};
	};

	loadNext();
}

// https://www.educative.io/edpresso/how-to-dynamically-load-a-js-file-in-javascript
function loadJS(FILE_URL, async = true, preGetV = undefined, postGetV = undefined) {
	let scE = d.createElement("script");
	scE.setAttribute("src", FILE_URL);
	scE.setAttribute("type", "text/javascript");
	scE.setAttribute("async", async);
	d.body.appendChild(scE);
	// success event 
	scE.addEventListener("load", () => {
		//console.log("File loaded");
		if (preGetV) preGetV();
		GetV();
		if (postGetV) postGetV();
	});
	// error event
	scE.addEventListener("error", (ev) => {
		console.log("Error on loading file", ev);
		alert("Loading of configuration script failed.\nIncomplete page data!");
	});
}
function getLoc() {
	let l = window.location;
	if (l.protocol == "file:") {
		loc = true;
		locip = localStorage.getItem('locIp');
		if (!locip) {
			locip = prompt("File Mode. Please enter WLED IP!");
			localStorage.setItem('locIp', locip);
		}
	} else {
		// detect reverse proxy
		let path = l.pathname;
		let paths = path.slice(1,path.endsWith('/')?-1:undefined).split("/");
		if (paths.length > 1) paths.pop(); // remove subpage (or "settings")
		if (paths.length > 0 && paths[paths.length-1]=="settings") paths.pop(); // remove "settings"
		if (paths.length > 1) {
			locproto = l.protocol;
			loc = true;
			locip = l.hostname + (l.port ? ":" + l.port : "") + "/" + paths.join('/');
		}
	}
}
// function getURL(path) { 

// 	console.log("getURL in common.js", loc, locip, path);
	
	
// 	return (loc ? locproto + "//" + locip : "") + path; 
// 	// 
	
// }
function getURL(path) {
	path = path || "/";
	if (path[0] != "/") path = "/" + path;
	return location.origin + path;
}
function B()          { window.open(getURL("/settings"),"_self"); }
function BL()          { window.open(getURL("/lights/settings"),"_self"); }
var timeout;
function showToast(text, error = false) {
	var x = gId("toast");
	if (!x) return;
	x.innerHTML = text;
	x.className = error ? "error":"show";
	clearTimeout(timeout);
	x.style.animation = 'none';
	timeout = setTimeout(function(){ x.className = x.className.replace("show", ""); }, 2900);
}
function uploadFile(fileObj, name) {
	var req = new XMLHttpRequest();
	req.addEventListener('load', function(){showToast(this.responseText,this.status >= 400)});
	req.addEventListener('error', function(e){showToast(e.stack,true);});
	req.open("POST", "/upload");
	var formData = new FormData();
	formData.append("data", fileObj.files[0], name);
	req.send(formData);
	fileObj.value = '';
	return false;
}

// connect to WebSocket, use parent WS or open new, callback function gets passed the new WS object
function connectWs(onOpen) {
	let ws;
	try {	ws = top.window.ws;} catch (e) {}
	// reuse if open
	if (ws && ws.readyState === WebSocket.OPEN) {
		if (onOpen) onOpen(ws);
	} else {
		// create new ws connection
		getLoc(); // ensure globals are up to date
		let url = loc ? getURL('/ws').replace("http", "ws")
									: "ws://" + window.location.hostname + "/ws";
		ws = new WebSocket(url);
		ws.binaryType = "arraybuffer";
		if (onOpen) ws.onopen = () => onOpen(ws);
	}
	return ws;
}



// Fetch GPIO caps (/settings/s.js?p=11) then pin occupancy (/json/pins) with retry.
// Caches result in d.pinsData. Calls cb() when ready (or on failure).
// If page already loaded its own s.js (d.max_gpio set), skips caps load and goes straight to pins fetch.
function fetchPinInfo(cb, retries=5) {
	if (d.pinsData) { cb&&cb(); return; }
	var done=false, fr=retries;
	function doFetch() {
		fetch(getURL('/json/pins'))
			.then(r=>r.json())
			.then(j=>{ if(!done){done=true; d.pinsData=j.pins||[]; cb&&cb();} })
			.catch(()=>{ fr-->0 ? setTimeout(doFetch,100) : (!done&&(done=true,d.pinsData=[],cb&&cb())); });
	}
	if (d.max_gpio) { doFetch(); return; }
	// Load GPIO caps from s.js?p=11 first (sets d.rsvd/ro_gpio/max_gpio/touch/adc/um_p)
	d.max_gpio=50; d.rsvd=[]; d.ro_gpio=[]; d.touch=[]; d.adc=[]; d.um_p=[];
	var cr=retries;
	function tryCaps() {
		var s=cE("script"); s.src=getURL('/settings/s.js?p=11');
		d.body.appendChild(s);
		s.onload=function(){ GetV(); doFetch(); };
		s.onerror=function(){ cr-->0 ? setTimeout(tryCaps,100) : doFetch(); };
	}
	tryCaps();
}


// Pin dropdown utilities
// Create or rebuild a pin <select> from an <input> or existing <select>
// name: form field name, requirement flags bitmask: 1=output, 2=touch, 4=ADC
function makePinSelect(name, flags) {
	let el = gN(name);
	if (!el) return null;
	let v = parseInt(el.value);
	if (isNaN(v)) v = -1;

	let sel;
	if (el.tagName === "SELECT") {
		sel = el;
		while (sel.lastChild) sel.lastChild.remove();
	} else {
		sel = cE('select');
		sel.classList.add("pin");
		sel.name = el.name;
		if (el.required) sel.required = true;
		let oc = el.getAttribute("onchange");
		if (oc) sel.setAttribute("onchange", oc);
		el.parentElement.replaceChild(sel, el);
	}

	let hasV = false;
	for (let j = -1; j < (d.max_gpio||0); j++) {
		if (j > -1 && d.rsvd && d.rsvd.includes(j)) continue;
		if (j > -1 && (flags & 1) && d.ro_gpio && d.ro_gpio.includes(j)) continue;
		if (j > -1 && (flags & 2) && (!d.touch || !d.touch.includes(j))) continue;
		if (j > -1 && (flags & 4) && (!d.adc || !d.adc.includes(j))) continue;

		let pInfo = d.pinsData && d.pinsData.find(p => p.p === j);
		let used = j > -1 && pInfo && pInfo.a && j !== v;
		let txt = j === -1 ? "unused" : `${j}`;
		if (used) txt += ` (${getOwnerName(pInfo.o, pInfo.t, pInfo.n)})`;
		// if (j > -1 && d.ro_gpio && d.ro_gpio.includes(j)) txt += " (R/O)"; // read only pins  note: removed as pin is not shown for outputs

		let opt = cE("option");
		opt.value = j;
		opt.text = txt;
		sel.appendChild(opt);

		if (j === v) { opt.selected = true; hasV = true; }
		else if (used) opt.disabled = true;
	}

	// Safety for invalid pins currently saved
	if (!hasV && v >= 0) {
		let opt = cE("option");
		opt.value = v; opt.text = v + " ⚠"; opt.selected = true;
		sel.insertBefore(opt, sel.options[1]);
	}
	sel.dataset.val = v;
	return sel;
}

// Convert pin <select> back to <input type="number">
function unmakePinSelect(name) {
	let sel = gN(name);
	if (!sel || sel.tagName !== "SELECT") return null;
	let inp = cE('input');
	inp.type = "number";
	inp.name = sel.name;
	inp.value = sel.value;
	inp.className = "s";
	if (sel.required) inp.required = true;
	let oc = sel.getAttribute("onchange");
	if (oc) inp.setAttribute("onchange", oc);
	sel.parentElement.replaceChild(inp, sel);
	return inp;
}