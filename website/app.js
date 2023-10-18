
function onload()
{
    setInterval(heartbeat, 100)
}

function onunload()
{
    fetch('/terminate', { method: "POST" })
}

async function heartbeat()
{
    response = await fetch('/values.txt', { method: "GET" })
    values_str = await response.text()
    values = values_str.split(';')

    for (idx = 0; idx < values.length; ++idx)
    {
        v = parseInt( values[idx] )
        document.getElementById('input'+idx).value = v
        document.getElementById('feedback'+idx).value = v
    }
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
    await fetch('value?'+label+'='+v, { method: "GET" })
}
