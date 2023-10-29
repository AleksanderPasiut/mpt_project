slider_busy_idx = -1

function onload()
{
    // setInterval(heartbeat, 200)
}

function onunload()
{
    fetch('/terminate', { method: "POST" })
}

function refreshCachedImage(img_id)
{
    var img = document.getElementById(img_id);
    img.src = img.src; // trick browser into reload
};

async function onstringchange()
{
    var v = document.getElementById('inputs').value;
    await fetch('/string?'+v, { method: "GET" })

    await fetch('/qr_code.bmp', { cache: 'reload', mode: 'no-cors' })
    refreshCachedImage('qr')
}
