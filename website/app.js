slider_busy_idx = -1

function onload()
{
    setInterval(heartbeat, 200)
    fetch('/initialize', { method: "POST" })
}

async function heartbeat()
{
    response = await fetch('/values.txt', { method: "GET" })
    values_str = await response.text()
    values = values_str.split(';')

    for (idx = 0; idx < values.length; ++idx)
    {
        if (idx != slider_busy_idx)
        {
            v = parseInt( values[idx] )
            document.getElementById('input'+idx).value = v
            document.getElementById('feedback'+idx).value = v
        }
    }

    response = await fetch('/string.txt', { method: "GET" })
    str = await response.text()
    document.getElementById('output').value = str
}

function onsliding(idx)
{
    slider_busy_idx = idx
    var v = document.getElementById('input'+idx).value;
    document.getElementById('feedback'+idx).value = v;
}

async function onslider(idx)
{
    slider_busy_idx = -1
    var label = 'input'+idx
    var v = document.getElementById(label).value;
    await fetch('value?'+label+'='+v, { method: "GET" })
}

async function onstringchange(idx)
{
    var v = document.getElementById('inputs'+idx).value;
    await fetch('/string'+idx,
    {
        method: "POST",
        mode: "cors",
        cache: "no-cache",
        headers:
        {
            "Content-Type": "text/plain",
        },
        redirect: "follow",
        body: v
    });
}

async function ontrigger()
{
    await fetch('/trigger',
    {
        method: "POST"
    });
}
