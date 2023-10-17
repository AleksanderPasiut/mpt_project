
function onload()
{
    // setInterval(heartbeat, 1000)
}

function onunload()
{
    fetch('/terminate', { method: "POST" })
}

var counter = 0;
function heartbeat()
{
    console.log(counter++)
}

function onsliding(idx)
{
    var v = document.getElementById('input'+idx).value;
    document.getElementById('feedback'+idx).value = v;
}

async function onslider(idx)
{
    var label = 'input'+idx
    var v = document.getElementById(label).value;
    fetch('value?'+label+'='+v, { method: "GET" })
}
