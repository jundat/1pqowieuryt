//Schedule functions


var listRoomType = [2];//, 5, 10, 15, 25];
				//send,play,submit,result,reset
var listTime = 	[5000, 0, 5000, 5000, 5000];
var listPlayTime = [10000];//, 10000, 15000, 15000, 15000];
var listBoardSize = [16];//, 16, 25, 25, 25];

var isStop = false;
var timeCounter = 0;
var hClock;

var hTimeout1, hTimeout2, hTimeout3, hTimeout4;


//SCHEDULE FUNCTIONS -------------------------------------------------------------------------------

function SchedulePushAllMatch(index) {
	if(isStop == false) {
		hTimeout1 = setTimeout(function(){
			ScheduleRefreshAllMatch(index);
		}, listTime[0]); //send time
	}

	PushAllMatch(listRoomType[index]); //send all match
	document.getElementById('log').value += strContent + '\n';
}


function ScheduleRefreshAllMatch(index) {
	if(isStop == false) {
		hTimeout2 = setTimeout(function(){
			SchedulePushAllResult(index);
		}, listPlayTime[index] + listTime[2]); //play + submit time ------- different --------------------
	}

	RefreshAllMatch(listRoomType[index], listBoardSize[index]); //refresh all match
	document.getElementById('log').value += strContent + '\n';
}


function SchedulePushAllResult(index) {
	if(isStop == false) {
		hTimeout3 = setTimeout(function(){
			ScheduleRefreshAllResult(index);
		}, listTime[3]); //submit time
	}

	PushAllResult(listRoomType[index]); //push all result
	document.getElementById('log').value += strContent + '\n';
}


function ScheduleRefreshAllResult(index) {
	if(isStop == false) {
		hTimeout4 = setTimeout(function(){
			SchedulePushAllMatch(index);
		}, listTime[4]); //reset time
	}

	RefreshAllResult(listRoomType[index]); //refresh all result
	document.getElementById('log').value += strContent + '\n';
}

//PUBLIC FUNCTIONS ---------------------------------------------------------------------------------

function StartServer() {
	isStop = false;
	timeCounter = 0;

	//prepare all match
	for (var i = listRoomType.length - 1; i >= 0; i--) {
		RefreshAllMatch(listRoomType[i], listBoardSize[i]);
	}

	//start
	setTimeout(function(){
		for (var i = listRoomType.length - 1; i >= 0; i--) {
			SchedulePushAllMatch(i); //index, not room type
		}
	}, 5000); //5s for preparation

	hClock = setInterval(function() {
		timeCounter++;
		document.getElementById('log').value += timeCounter + '\n';// + '\t:' + strContent + '\n';
		//document.getElementById('status').innerHTML = timeCounter + '\t:' + strContent + '\n';
	}, 1000);
}


function StopServer() {
	isStop = true;
	clearInterval(hClock);
	clearInterval(hTimeout4);
	clearInterval(hTimeout3);
	clearInterval(hTimeout2);
	clearInterval(hTimeout1);
}

function Clear() {
	document.getElementById('log').value = "";
}

