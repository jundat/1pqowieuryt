//MATCH GENERATION FUNCTIONS


//private functions -------------------------------------

function toAtom(word){
    var atom = {};
    for(var i=0;i<word.length;i++){
        if(atom[word[i]]==null){
            atom[word[i]] = 1;
        } else {
            atom[word[i]] += 1;
        }
    }
    
    return atom;
}

function toArray(atom) {
    var array = [];
    for(var key in atom){
        for(var i=0;i<atom[key];i++)
            array.push(key);
    }
    return array;
}

function mergeAtom(src1, src2, dst) {
    var count = 0;
    for( var key in src1) {
        if(src2[key]==null || src1[key]>src2[key]){
            dst[key] = src1[key];
        }else{
            dst[key] = src2[key];
        }
        count += dst[key];
    }
    for( var key in src2) {
        if(dst[key] == null){
            dst[key] = src2[key];
            count += dst[key];
        }       
    }
    return count;
}

function mergeAtomTo(src, dst) {
    for(var key in src){
        if(dst[key] == null || dst[key]<src[key])
            dst[key] = src[key];
    }
}

function excluseAtom(src, excluse){
    var ex = {};
    for( var key in src) {
        var value = (excluse[key] != null)?(src[key] - excluse[key]):src[key];
        if (value > 0)
            ex[key] = value;
    }
    return ex;
}

function diff(atom1,atom2){
    var diff = {};
    for (var key in atom1){
        if(atom2[key] == null)
            diff[key] = atom1[key];
        else if (atom1[key] != atom2[key])
            diff[key] = (atom1[key] > atom2[key])?(atom1[key]-atom2[key]):(atom2[key]-atom1[key]);
    }
    for (var key in atom2){
        if(diff[key] == null && atom1[key] == null)
            diff[key] = atom2[key]
    }
    return diff;
}

function diffFrom(from, atom){
    var diff = {}
    for (var key in from){
        if(atom[key] == null)
            diff[key] = from[key];
        else if(from[key] > atom[key])
            diff[key] = from[key] - atom[key];
    }
    return diff;
}

function similar(atom1,atom2){
    var sml = {};
    for (var key in atom1){
        if(atom2[key] != null)
            sml[key] = Math.min(atom1[key],atom2[key]);
    }
    return sml;
}

//return true if src contain atom
function isContainAtom(atom, src){
    for(var key in atom) {
        if(src[key]==null || atom[key]>src[key]){
            return false;
        }
    }   
    return true;
}

//Return true if a word is prefix of the other
function checkPrefix(word1, word2){
    if(word1.length >= word2.length){
        return word1.search(word2) == 0;
    }else{
        return word2.search(word1) == 0;
    }
    return true;
}

//Generate the board from the must-have character
function genBoard(atomList, disChar, size) {
    var pos = 0;
    var board = [];
    //pass all must-have character to board
    for(var key in atomList) {
        while(atomList[key] > 0){
            board[pos] = key;
            atomList[key] -= 1;
            pos += 1;
        }
    }

    //Add additional characters
    var disable = ""
    for(var i=0;i<disChar.length;i++)
        disable = disable + disChar[i];
    while(pos < size) {
        var ch = null;
        var invalid = false;
        do {   
            ch = String.fromCharCode(97+Math.floor(Math.random()*25));
        } while( disable.search(ch) != -1 );
        board[pos] = ch;
        pos +=1;
    }
 
    return board;
}

//Shuffle the board
function boardShuffle(board) {
    var size = board.length
    var i = 0
    var j = size-1
    while(j>=0) {
        var t = board[i]
        board[i]=board[j]
        board[j]=t
        i++
        j-=2
    }
}

//return the difficult for specify level
function GetDifficult(roomType){
//return {min_length, max_length, boardsize, trap, hidden, time, accept_time}
    switch(roomType){
        case 2:return [4,6,16,2,5,15000,7000];
        break;
    case 5:
        return [7,9,16,1,7,15000,5000];
        break;
    case 10:
        return [7,9,25,2,7,20000,10000];
        break;
    case 15:
        return [9,12,25,1,9,20000,10000];
        break;
    case 25:
        return [9,12,25,2,12,20000,10000];
        break;
    default:
        return [4,6,16,1,0,10000,10000];
        break;
    }
}

function GenRegex(word){
    //method = 0: match prefix
    //         1: match subfix
    //         2: match postfix


    var method = Math.floor(Math.random()*2.8);

    var regex = "";

    regexlength = function(w) {

        var length = w.length;
        var take = 0
        var shift = 0
        if (length >= 4 && length <= 6) {
            take = Math.floor(1 + Math.random()*2);
            shift = Math.floor(1 + Math.random()*(length - take - 1.2) );
            return [take,shift];
        }
        else if (length >= 7 && length <= 9) {
            take = Math.floor(2 + Math.random()*2);
            shift = Math.floor(1 + Math.random()*(length - take - 1.2) );
            return [take,shift];
        }
        else if (length >= 7 && length <= 9) {
            take = Math.floor(3 + Math.random()*2);
            shift = Math.floor(1 + Math.random()*(length - take - 1.2) );
            return [take,shift];
        }
        else {
            return 0;
        }
    }

    switch(method){
        case 0:
            prefix = function(word, regex_length){
                var prefix = word.substr(0,regex_length[0]);
                return prefix;
            }
            regex = prefix(word,regexlength(word));
            break;
        case 1:
            subfix = function(word, regex_length){
                var subfix = word.substr(regex_length[1],regex_length[0]);
                return  subfix;
            }
            regex = subfix(word,regexlength(word));
            break;
        case 2:
            postfix = function(word, regex_length){
                var postfix = word.substr(word.length - regex_length[0], regex_length[0]);
                return  postfix;
            }
            regex = postfix(word,regexlength(word));
            break;
        default:
            break;
    }
    return [method,regex];
}


//end private functions -------------------------------------





//roomType
//matchId
//word0
Parse.Cloud.define('generateMatch', function(req, res) {
    var roomType = req.params.roomType;
    var matchId = req.params.matchId;
    var word0 = req.params.word0;

    //----
    var difficult = GetDifficult(roomType)
 
    var min_length = difficult[0];
    var max_length = difficult[1];
    var boardSize = difficult[2];
    var trap = difficult[3];
    var hidden = difficult[4];
    var time = difficult[5]

    var word = [];
    var atom = [];
    var charList = {}
    var tmp = {}

    // TODO Get 1st word here with min & max length
    word[0] = word0;//
    atom[0] = toAtom(word[0]);
 
    var regex = GenRegex(word[0])
    var queryMethod = regex[0] //method to find {prefix,subfix,postfix}
    var substr = regex[1] //find words contain this value

    var query = new Parse.Query('Words');
    query.greaterThan('len', min_length - 1);
    query.lessThan('len', max_length + 1);

    /* TODO query
         use min & max length,
         use query method
         case 0: parse.query.startsWith("word", substr)
         case 1: parse.query.contains("word", substr)
         case 2: parse.query.endsWith("word", substr)
    */

    switch(queryMethod) {
        case 0:
            query.startsWith('english', substr);
        break;

        case 1:
            query.contains('english', substr);
        break;

        case 2:
            query.endsWith('english', substr);
        break;
    }

    query.find({
        success: function(results) { //-------------------------------------------------------------
            // random select 2nd word from query
            do {
                var idx = Math.floor(Math.random() * results.length);

                word[1] = results[idx].get('english');// "chat"; //random from query-----------
                tmp = {};
                atom[1] = toAtom(word[1]);
            } while (checkPrefix(word[0],word[1]) || (mergeAtom(atom[0],atom[1],tmp) > boardSize));

            mergeAtom(atom[0],atom[1],charList);

            // random select 3rd word from query
            do {
                var idx = Math.floor(Math.random() * results.length);

                word[2] = results[idx].get('english');// "hat"; //random from query-------------
                tmp = {};
                atom[2] = toAtom(word[2]);
            } while (checkPrefix(word[0],word[2]) || checkPrefix(word[1],word[2]) || (mergeAtom(atom[2],charList,tmp) > boardSize));

            mergeAtom(atom[2],charList,tmp);
            charList = tmp;

            //removeChar contain letter to be delete from charList
            var possibleChar = [];
         
            if(trap >= 1) {
                var sml = toAtom(substr);
                var a = [];
                for(var i=0;i<3;i++){
                    a[i] = excluseAtom(atom[i],sml);
                }

                var disChar = {}
                for(var i=0;i<3;i++){
                    var d01 = diffFrom(a[i%3],a[(i+1)%3]);
                    var d012 = diffFrom(d01,a[(i+2)%3]);
                    mergeAtomTo(d012,disChar)
                }

                for(var key in disChar){
                    for(var i=0;i<disChar[key];i++){
                        possibleChar.push(key);
                    }
                }
            }

            var removeChar = []

            if(trap == 1){
                if(possibleChar.length > 0){
                    removeChar.push(possibleChar[Math.floor(Math.random()*possibleChar.length)]);
                } else {

                    var disChar = {}
                    for(var i=0;i<3;i++){
                        var s01 = similar(a[i],a[(i+1)%3]);
                        var s02 = similar(a[i],a[(i+2)%3]);
                        var d12 = diff(s01,s02);
                        mergeAtomTo(d12,disChar);
                    }

                    for(var key in disChar) {
                        for(var i=0;i<disChar[key];i++) {
                            possibleChar.push(key);
                        }
                    }
                    removeChar.push(possibleChar[Math.floor(Math.random()*possibleChar.length)]);
                }
            }else if(trap == 2){

                if(possibleChar.length > 1){
                    var i = Math.floor(Math.random()*3);
                    var p = excluseAtom(disChar,a[i]);
                    var e1 = toArray(excluseAtom(p,a[(i+1)%3]));
                    var e2 = toArray(excluseAtom(p,a[(i+2)%3]));
                    if(e1.length > 0)
                        removeChar.push(e1[Math.floor(Math.random()*e1.length)]);
                    if(e2.length > 0)
                        removeChar.push(e2[Math.floor(Math.random()*e2.length)]);
                }

                if(possibleChar.length <= 1 || removeChar.length < 1){
                    var disChar = {}
                    
                    for(var i=0;i<3;i++){
                        var s01 = similar(a[i],a[(i+1)%3]);
                        var s02 = similar(a[i],a[(i+2)%3]);
                        var d12 = diff(s01,s02);
                        mergeAtomTo(d12,disChar);
                    }

                    for(var key in disChar) {
                        for(var i=0;i<disChar[key];i++) {
                            possibleChar.push(key);
                        }
                    }
                    removeChar.push(possibleChar[Math.floor(Math.random()*possibleChar.length)]);
                }
            }

            for(var i=0;i<removeChar.length;i++) {
                charList[removeChar[i]] -= 1;
            }

            var board = genBoard(charList,removeChar,boardSize)
            boardShuffle(board)
         
            var obj = {
                matchId: matchId,
                board: board, 
                word: word,
                hidden: hidden,
                time: time
            }

            res.success(obj);
            //--------------------------------------------------------------------------------------------------
        },
        error: function(error) {
            res.error('Can not query word1 + word2');
        }
    });
});



Parse.Cloud.define('RefreshMatchInRoomType', function(req, res){
    var roomType = req.params.roomType;

    var query1 = new Parse.Query('RoomType');
    query1.equalTo('type', roomType);
    query1.first({
        success: function(result1) {
            var newMatchId = result1.get('matchId') + 1;

            var difficult = GetDifficult(roomType);
            var min_length = difficult[0];
            var max_length = difficult[1];
            
            var query2 = new Parse.Query('Words');
            query2.greaterThan('len', min_length - 1);
            query2.lessThan('len', max_length + 1);
            //query2.limit(1000);

            query2.find({
                success: function(results2) { //-------------
                    var idx = Math.floor(Math.random() * results2.length);
                    var word0 = results2[idx].get('english');

                    Parse.Cloud.run('generateMatch', {
                        roomType: roomType,
                        matchId: newMatchId,
                        word0: word0
                    }, {
                        success: function(result3) {
                            //save new matchId
                            result1.set('matchId', newMatchId); //increase matchId
                            result1.set('match', JSON.stringify(result3));
                            result1.save();
                        },
                        error: function(error3) { res.error('can not generateMatch: ' + JSON.stringify(error3)); }
                    });
                },
                error: function(error2) { res.error('can not get word0: ' + JSON.stringify(error2)); }
            });
        },
        error: function(error1) { res.error('can not find in room :' + roomType); }
    });
});


//called from AutoServer
//input: roomType
//generate new match, save replace for all room at room type
Parse.Cloud.define('refreshAllMatch',
    function(req, res) {
        var roomType = req.params.roomType;

        Parse.Cloud.run('RefreshMatchInRoomType', {
            roomType: roomType
        }, {
            success: function(result) {
                //res.success(result);
            },
            error: function(error) { 
                //res.error('can not RefreshMatchInRoomType' + JSON.stringify(error)); 
            }
        });
});