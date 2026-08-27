var d=document;
var loc = false, locip, locproto = "http:";
var __alertActive = false;

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
function toggle(el) { gId(el).classList.toggle("hide"); gId('No'+el).classList.toggle("hide"); }
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
	// scE.addEventListener("error", (ev) => {
	// 	console.log("Error on loading file", ev);
	// 	alert("Loading of configuration script failed.\nIncomplete page data!");
	// });
	scE.addEventListener("error", (ev) => {
		console.log("Error on loading file", FILE_URL, ev);

		// Show alert only once
		if (!__alertActive) {
			__alertActive = true;
			alert("Loading of configuration script failed.\nIncomplete page data!");
		}
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
// 	console.log("getURL in web:common.js", loc, locip, path);
	
// 	return (loc ? locproto + "//" + locip : "") + path; }


function getURL(path) {
	path = path || "/";
	if (path[0] != "/") path = "/" + path;
	return location.origin + path;
}


function B()          { window.open(getURL("/"),"_self"); }
function B2()          { window.open(getURL("/settings2"),"_self"); }
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



// Date Modified: 25Jan26
function SHTML(id, html){ var e=gId(id); if(e) e.innerHTML = html; }
// function STXT(id, txt){ var e=gId(id); if(e) e.textContent = txt; }
// function SCLS(id, cls, en){ var e=gId(id); if(e) e.classList.toggle(cls, !!en); }
// function SATTR(id, k, v){ var e=gId(id); if(e) e.setAttribute(k, v); }

// // “build helpers” (optional)
// function KVRow(k, v){
//   return "<tr><td class='key'>" + k + "</td><td class='val'>" + v + "</td></tr>";
// }
// function KVTable(rowsHtml){
//   return "<table class='kv'>" + rowsHtml + "</table>";
// }
// function Section(title, innerHtml){
//   return "<div class='section'><div class='title'>" + title + "</div>" + innerHtml + "</div>";
// }




// -----------------------------------------------------------------------------
// Generic 3-column table helpers added April2026
//   - table id convention:
//       table body id = "<tableId>_body"
//       cell ids      = "<tableId>_r<row>c<col>"
//   - intended columns:
//       c0 = name
//       c1 = value
//       c2 = notes
// -----------------------------------------------------------------------------

function TID(tableId, row, col){
  return tableId + "_r" + row + "c" + col;
}

function TClear(tableId){
  SHTML(tableId + "_body", "");
}

function TRow(tableId, row, c0 = "", c1 = "", c2 = ""){
  var body = gId(tableId + "_body");
  if (!body) return;

  var tr  = cE("tr");
  var td0 = cE("td");
  var td1 = cE("td");
  var td2 = cE("td");

  td0.id = TID(tableId, row, 0);
  td1.id = TID(tableId, row, 1);
  td2.id = TID(tableId, row, 2);

  td0.innerHTML = c0;
  td1.innerHTML = c1;
  td2.innerHTML = c2;

  tr.appendChild(td0);
  tr.appendChild(td1);
  tr.appendChild(td2);
  body.appendChild(tr);
}

function TSet(tableId, row, col, html){
  SHTML(TID(tableId, row, col), html);
}

function TSetName(tableId, row, html){
  TSet(tableId, row, 0, html);
}

function TSetValue(tableId, row, html){
  TSet(tableId, row, 1, html);
}

function TSetNotes(tableId, row, html){
  TSet(tableId, row, 2, html);
}


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
