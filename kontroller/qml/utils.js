.pragma library

function formatTime(time)
{
    var ret = "";
    var hour = Math.floor(time / 3600);
    if(hour > 0)
        ret = hour + "h";
    var min = Math.floor((time - hour * 3600) / 60);
    if(hour > 0 || min > 0)
    {
        if(hour > 0 && min < 10)
            ret += "0";
        ret += min;
        if(hour == 0)
            ret += "min";
    }
    if(hour == 0)
    {
        var sec = time % 60;
        if(min > 0 && sec < 10)
            ret += "0";
        ret += sec;
    }
    return ret;
}

function formatRating(rating)
{
    var rate = Math.floor(rating);
    var left = Math.floor(rating * 10 - rate * 10);
    return "" + rate + "." + left;
}

function formatDate(date)
{
    return date.toLocaleDateString(Qt.locale(), 1);
}

function join(vals, sep)
{
    if(vals.length == 0)
        return "";
    var ret = vals[0];
    for(var i = 1; i < vals.length; ++i)
    {
        ret += ', ' + vals[i];
    }
    return ret;
}

function formatMsecTime(time)
{
    var timeInSec = Math.floor(time / 1000);
    if(timeInSec < 0)
        return "0:00";
    var secs = timeInSec % 60;
    var timeInMins = Math.floor(timeInSec / 60);
    var mins = timeInMins % 60;
    var hours = Math.floor(timeInMins / 60);
    var ret = "";
    if(hours > 0)
        ret += hours + ":";
    if( mins < 10 && hours > 0)
        ret += "0";
    ret += mins + ":";
    if(secs < 10)
        ret += "0"
    ret += secs;
    return ret;
}

function formatArray(strings)
{
    var ret = "";
    for(var i = 0; i < strings.length; ++i)
    {
        if(i !== 0)
            ret += ", ";
        ret += strings[i];
    }
    return ret;
}
