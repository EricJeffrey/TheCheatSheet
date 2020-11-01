import axios from 'axios';

const BASE_URL = "/codeSegment/";
const DEFAULT_PAGE_SIZE = 20;

// const MOCK_CODE_SEGMENTS = [
//     {
//         "id": "ea98f819-0dde-4e25-a6a2-17314cc27f1c",
//         "title": "Linux创建空文件",
//         "description": "",
//         "code": "touch a.txt\nextra={\n<Space>\n<EyeOutlined onClick={() => { this.viewDetail(index, false); }}"
//             + "/>\n<EditOutlined onClick={() => { this.viewDetail(index, true); }} />",
//         "tag": "Command",
//         "createAt": "1603092591383",
//         "lastModify": "1603092601384"
//     },
//     {
//         "id": "ea98f819-0dde-4e25-a6a2-17314cc27f1c",
//         "title": "Linux创建空文件",
//         "description": "",
//         "code": "#include <cstdio> \n int main(){\n\tprintf(\"hello world\"); return 0;\n}",
//         "tag": "C++",
//         "createAt": "1603092591383",
//         "lastModify": "1603092601384"
//     },
//     {
//         "id": "ea98f819-0dde-4e25-a6a2-17314cc27f1c",
//         "title": "Linux创建空文件",
//         "description": "",
//         "code": "touch a.txt",
//         "tag": "Command",
//         "createAt": "1603092591383",
//         "lastModify": "1603092601384"
//     }
// ];

const client = {
    getCodeSegments: function (pageCount) {
        return new Promise((resolve, reject) => {
            axios.get(BASE_URL + "all", {
                params: {
                    pageSize: DEFAULT_PAGE_SIZE,
                    pageCount: pageCount
                }
            }).then((response) => {
                if (response.status === 200) resolve(response.data);
                else reject(response.statusText);
            }).catch((reason) => reject(reason));
        });
    },
    getCodeSegmentsByTag: function (tag, pageCount) {
        return new Promise((resolve, reject) => {
            axios.get(BASE_URL + "all", {
                params: {
                    tag: tag,
                    pageSize: DEFAULT_PAGE_SIZE,
                    pageCount: pageCount
                }
            }).then((response) => {
                if (response.status === 200) resolve(response.data);
                else reject(response.statusText);
            }).catch((reason) => reject(reason))
        });
    },
    getAllTags: function () {
        return new Promise((resolve, reject) => {
            axios.get(BASE_URL + "allTags")
                .then((response) => {
                    if (response.status === 200) resolve(response.data);
                    else reject(response.statusText);
                }).catch((reason) => reject(reason));
        });
    },
    searchCodeSegments: function (text, pageCount) {
        return new Promise((resolve, reject) => {
            axios.get(BASE_URL + "search", {
                params: {
                    text: text,
                    pageSize: DEFAULT_PAGE_SIZE,
                    pageCount: pageCount
                }
            }).then((response) => {
                if (response.status === 200) resolve(response.data);
                else reject(response.statusText);
            }).catch((reason) => reject(reason));
        });
    },
    updateCodeSegment: function (codeSegment) {
        return new Promise((resolve, reject) => {
            axios.post(BASE_URL + "update", codeSegment, {
                headers: { 'Content-Type': 'application/json' }
            }).then((response) => {
                if (response.status === 200) resolve(response.data);
                else reject(response.statusText);
            }).catch((reason) => reject(reason));
        });
    },
    addCodeSegment: function (title, description, tag, code) {
        return new Promise((resolve, reject) => {
            const data = { title: title, description: description, code: code, tag: tag };
            axios.post(BASE_URL + "add", data, {
                headers: { 'Content-Type': 'application/json' }
            }).then((response) => {
                if (response.status === 200) resolve(response.data);
                else reject(response.statusText);
            }).catch((reason) => reject(reason));
        });
    }
};
export default client;