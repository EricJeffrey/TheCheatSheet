import React, { Component } from 'react';
import { Button, Card, Input, Layout, Menu, Select, Space } from 'antd';
import SyntaxHighlighter from 'react-syntax-highlighter';
import {
    EyeOutlined, CopyOutlined, EditOutlined,
    CloseCircleOutlined, CheckCircleOutlined
} from '@ant-design/icons';
import 'antd/dist/antd.css';
import './css/App.css';
import Modal from 'antd/lib/modal/Modal';
import TextArea from 'antd/lib/input/TextArea';
import { docco } from 'react-syntax-highlighter/dist/esm/styles/hljs';

const { Header, Sider, Content } = Layout;

// todo load pageCount/pagesize ?
export default class App extends Component {
    constructor(props) {
        super(props);

        this.state = {
            dialogStatus: {
                visible: false,
                editable: false,
                adding: false,
                data: {
                    id: "", title: "", desc: "", code: "",
                    language: "", createAt: "", lastModify: ""
                }
            },

            langIndex: 0,
            langlist: [
                "All", "Bash", "Java", "Php", "C#"
            ],
            data: [
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
                    "id": "eas8f819-gddd-4e35-a612-17314c327f1c",
                    "title": "Java日期格式化",
                    "desc": "使用SimpleDateFormat类格式化当前日期并打印\n日期格式为xxxx年xx月xx日星期x xx点xx分xx秒",
                    "code": "Date now = new Date(); // 创建一个Date对象，获取当前时间\nSimpleDateFormat f = new SimpleDateFormat(\"yyyy年MM月dd日 HH点mm分ss秒\");\nSystem.out.println(f.format(now)); // 将当前时间袼式化为指定的格式",
                    "language": "Java",
                    "createAt": "1603092591383",
                    "lastModify": "1603092601384"
                }
            ],
            menuSelectedKey: "0",
        };

        this.onMenuClick = this.onMenuClick.bind(this);
        this.viewDetail = this.viewDetail.bind(this);
        this.onSearchPressEnter = this.onSearchPressEnter.bind(this);
        this.onAddClick = this.onAddClick.bind(this);
    }

    componentDidMount() {
        // todo init all list here
    }

    onSearchPressEnter(e) {
        const txt = e.target.value;
        if (txt == null || txt.length === 0) return;
        // todo do search here
    }

    onAddClick(e) {
        this.setState({
            dialogStatus: {
                visible: true,
                adding: true,
                editable: true,
                data: {
                    id: "", title: "", desc: "", code: "",
                    language: "", createAt: "", lastModify: ""
                }
            }
        });
    }

    onMenuClick({ item, key, keyPath, domEvent }) {
        // todo get all?language=key here
        this.setState({ menuSelectedKey: key });
    }

    viewDetail(index, doEdit) {
        this.setState({
            dialogStatus: {
                visible: true,
                adding: false,
                editable: doEdit,
                data: this.state.data[index]
            }
        });
    }

    doCopy(index) { }

    render() {
        // todo get all lang type here
        const menuItems = <>
            {this.state.langlist.map((value, index) =>
                <Menu.Item key={index.toString(10)}>{value}</Menu.Item>
            )}
        </>;
        const cardLists = <>
            {this.state.data.map((value, index) => {
                var descDiv;
                if (value.desc === null || value.desc.length === 0)
                    descDiv = <p>{value.title}</p>;
                else {
                    descDiv = <div>
                        {value.desc.split('\n').map((tmpv, index) => <p key={index}>
                            {tmpv}
                        </p>)}
                    </div>;
                }
                return <Card className="main_card"
                    title={
                        <div style={{ marginRight: "1em" }}>
                            {value.title}
                        </div>}
                    key={index}
                    extra={
                        <Space>
                            <EyeOutlined onClick={() => { this.viewDetail(index, false); }} />
                            <EditOutlined onClick={() => { this.viewDetail(index, true); }} />
                            <CopyOutlined onClick={() => { this.doCopy(index); }} />
                        </Space>
                    }>
                    {descDiv}
                    <pre>{value.code.split('\n')[0] + "..."}</pre>
                </Card>
            })}
        </>;

        return (
            <Layout className="comon_theme" id="main">
                <Header className="comon_theme" id="main_header">
                    <a href="/" id="main_logo">TheCheatsheet</a>
                    <div id="main_search_holder">
                        <Input
                            id="main_input_search"
                            onPressEnter={this.onSearchPressEnter}
                            placeholder="search" />
                        <div className="middle_space"></div>
                        <Button
                            id="main_btn_add"
                            onClick={this.onAddClick} >添加</Button>
                    </div>
                </Header>
                <Layout style={{ width: "100%" }}>
                    <Sider className="comon_theme" id="main_sider" theme="light">
                        <Menu
                            id="main_menu"
                            onClick={this.onMenuClick}
                            selectedKeys={this.state.menuSelectedKey}>
                            {menuItems}
                        </Menu>
                    </Sider>
                    <Content className="comon_theme" id="main_content" >
                        {cardLists}
                    </Content>
                    <DetailModal status={this.state.dialogStatus} />
                </Layout>
            </Layout>
        );
    }
};

class DetailModal extends Component {
    constructor(props) {
        super(props);

        // todo lang list should be fixed, create from this
        this.langlist = ["Android", "Bash", "C", "C++", "C#", "Dart", "Go", "Java",
            "JavaScript", "Kotlin", "Python", "Php", "Rust", "TypeScript",];
        this.status = props.status;

        this.onClose = this.onClose.bind(this);
        this.onDoEdit = this.onDoEdit.bind(this);
        this.onLangTypeChange = this.onLangTypeChange.bind(this);
        this.onComplete = this.onComplete.bind(this);
        this.onDescChange = this.onDescChange.bind(this);
        this.onCodeChange = this.onCodeChange.bind(this);
        this.onTitleChange = this.onTitleChange.bind(this);
    }
    onLangTypeChange(value, option) {
        console.log(value);
        this.status.data.language = value;
        this.setState({});
    }

    onDoEdit(e) {
        if (!this.status.editable) {
            this.status.editable = true;
            this.setState({});
        }
    }

    onComplete(e, close) {
        // todo add/update here
        if (close == null || !close) {
            this.status.editable = false;
            this.setState({});
        }
        else
            this.onClose();
    }

    onClose() {
        this.status.visible = false;
        this.setState({});
    }

    onDescChange(e) {
        this.status.data.desc = e.target.value;
        this.setState({});
    }

    onTitleChange(e,) {
        this.status.data.title = e.target.value;
        this.setState({});
    }

    onCodeChange(e) {
        this.status.data.code = e.target.value;
        this.setState({});
    }

    render() {
        this.status = this.props.status;
        const disabled = !this.status.editable;
        var rawlang = this.status.data.language;
        if (rawlang.length === 0) rawlang = "Java";

        var language = rawlang;
        language = language.toLowerCase();
        if (language === "c++") language = "cpp";
        if (language === "c#") language = "csharp";


        return <Modal
            onOk={(e) => { this.onComplete(e, true) }}
            onCancel={this.onClose}
            className="main_modal"
            visible={this.status.visible}
            closable={false}>
            <div className="main_modal_content">
                <Space style={{
                    width: "100%",
                    textAlign: "end",
                    display: "inline-flex",
                    justifyContent: "flex-end",
                    marginBottom: "1em"
                }}>
                    <CheckCircleOutlined hidden={disabled} onClick={this.onComplete} />
                    <EditOutlined onClick={this.onDoEdit} />
                    <CloseCircleOutlined onClick={this.onClose} />
                </Space>
                <div className="comon_theme"
                    style={{ marginBottom: "1em" }}>
                    <Input
                        disabled={disabled}
                        placeholder="标题"
                        onChange={this.onTitleChange}
                        value={this.status.data.title} />
                    <div className="small_space" />
                    <Select
                        style={{ width: "7em" }}
                        disabled={disabled}
                        defaultValue={rawlang}
                        onChange={this.onLangTypeChange}>
                        {this.langlist.map((value, index) =>
                            <Select.Option value={value} key={index}>
                                {value}
                            </Select.Option>)}
                    </Select>
                </div>
                <TextArea
                    disabled={disabled}
                    placeholder="代码段描述"
                    onChange={this.onDescChange}
                    style={{ marginBottom: "1em" }}
                    value={this.status.data.desc} />
                <div
                    className="common_theme main_modal_code"
                    style={{ marginBottom: "1em" }}>
                    <TextArea
                        placeholder="代码"
                        hidden={disabled}
                        value={this.status.data.code}
                        onChange={this.onCodeChange}
                        className="main_modal_code_editor" />
                    <SyntaxHighlighter
                        className="main_modal_highlighter"
                        style={docco}
                        language={language}
                        showLineNumbers>{this.status.data.code}
                    </SyntaxHighlighter>
                </div>
            </div>
        </Modal>
    }
}