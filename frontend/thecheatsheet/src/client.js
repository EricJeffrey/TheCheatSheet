import axios from 'axios';

function getCodeSeg(all, language, pageCnt, pageSz) {
    var paraList = [];
    if (language != null)
        paraList.push("language=" + language);
    if (pageCnt != null)
        paraList.push("pageCount=" + pageCnt);
    if (pageSz)
        paraList.push("pageSize=" + pageSz);
    var para = "";
    for (let i = 0; i < paraList.length; i++) {
        para += paraList[i];
        if (i != paraList.length - 1) para += "&";
    }
    var url = "/codeseg/all" + (para.length > 0 ? "?" + para : "");
    return new Promise((resolve, reject) => {
        resolve([
            {
                "id": "ea98f819-0dde-4e25-a6a2-17314cc27f1c",
                "title": "Linux创建空文件",
                "desc": "",
                "code": "touch a.txt",
                "language": "Command",
                "createAt": "1603092591383",
                "lastModify": "1603092601384"
            },
            {
                "id": "ea98f819-0dde-4e25-a6a2-17314cc27f1c",
                "title": "Linux创建空文件",
                "desc": "",
                "code": "touch a.txt",
                "language": "Command",
                "createAt": "1603092591383",
                "lastModify": "1603092601384"
            }
        ]);
    });
}