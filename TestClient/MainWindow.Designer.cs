namespace TestClient
{
    partial class MainWindow
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainWindow));
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.connectButton = new System.Windows.Forms.Button();
			this.port = new System.Windows.Forms.TextBox();
			this.label2 = new System.Windows.Forms.Label();
			this.ipAddress = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.writeTextBox = new System.Windows.Forms.TextBox();
			this.radioButtonHex = new System.Windows.Forms.RadioButton();
			this.radioButtonUTF8 = new System.Windows.Forms.RadioButton();
			this.radioButtonASCII = new System.Windows.Forms.RadioButton();
			this.sendButton = new System.Windows.Forms.Button();
			this.groupBox3 = new System.Windows.Forms.GroupBox();
			this.radioButtonReadHex = new System.Windows.Forms.RadioButton();
			this.readTextBox = new System.Windows.Forms.RichTextBox();
			this.radioButtonReadUTF8 = new System.Windows.Forms.RadioButton();
			this.radioButtonReadASCII = new System.Windows.Forms.RadioButton();
			this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.tabPage2 = new System.Windows.Forms.TabPage();
			this.groupBox5 = new System.Windows.Forms.GroupBox();
			this.apoapseMsgContent = new System.Windows.Forms.TextBox();
			this.buttonSendApoapseMsg = new System.Windows.Forms.Button();
			this.groupBox4 = new System.Windows.Forms.GroupBox();
			this.textBoxRichUsername = new System.Windows.Forms.TextBox();
			this.label5 = new System.Windows.Forms.Label();
			this.richConnectButton = new System.Windows.Forms.Button();
			this.portRich = new System.Windows.Forms.TextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.ipAddressRich = new System.Windows.Forms.TextBox();
			this.label4 = new System.Windows.Forms.Label();
			this.tabPage1 = new System.Windows.Forms.TabPage();
			this.statusStrip1 = new System.Windows.Forms.StatusStrip();
			this.connectedStatus = new System.Windows.Forms.ToolStripStatusLabel();
			this.buttonDisconnect = new System.Windows.Forms.ToolStripSplitButton();
			this.mainList = new System.Windows.Forms.ListView();
			this.columnTime = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnDirection = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnSize = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnText = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHex = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.button1 = new System.Windows.Forms.Button();
			this.textBoxRichDestination = new System.Windows.Forms.TextBox();
			this.label6 = new System.Windows.Forms.Label();
			this.groupBox1.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.groupBox3.SuspendLayout();
			this.tableLayoutPanel1.SuspendLayout();
			this.tabControl1.SuspendLayout();
			this.tabPage2.SuspendLayout();
			this.groupBox5.SuspendLayout();
			this.groupBox4.SuspendLayout();
			this.tabPage1.SuspendLayout();
			this.statusStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupBox1
			// 
			this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox1.Controls.Add(this.connectButton);
			this.groupBox1.Controls.Add(this.port);
			this.groupBox1.Controls.Add(this.label2);
			this.groupBox1.Controls.Add(this.ipAddress);
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Location = new System.Drawing.Point(6, 6);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(742, 48);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "TCP Connect (direct)";
			// 
			// connectButton
			// 
			this.connectButton.ForeColor = System.Drawing.Color.ForestGreen;
			this.connectButton.Location = new System.Drawing.Point(423, 15);
			this.connectButton.Name = "connectButton";
			this.connectButton.Size = new System.Drawing.Size(85, 23);
			this.connectButton.TabIndex = 4;
			this.connectButton.Text = "Connect";
			this.connectButton.UseVisualStyleBackColor = true;
			this.connectButton.Click += new System.EventHandler(this.connectButton_Click);
			// 
			// port
			// 
			this.port.AutoCompleteCustomSource.AddRange(new string[] {
            "3000",
            "55056"});
			this.port.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
			this.port.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.CustomSource;
			this.port.Location = new System.Drawing.Point(326, 16);
			this.port.Name = "port";
			this.port.Size = new System.Drawing.Size(78, 20);
			this.port.TabIndex = 3;
			this.port.Text = "3000";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(291, 19);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(29, 13);
			this.label2.TabIndex = 2;
			this.label2.Text = "Port:";
			// 
			// ipAddress
			// 
			this.ipAddress.Location = new System.Drawing.Point(32, 16);
			this.ipAddress.Name = "ipAddress";
			this.ipAddress.Size = new System.Drawing.Size(232, 20);
			this.ipAddress.TabIndex = 1;
			this.ipAddress.Text = "127.0.0.1";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(6, 19);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(20, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "IP:";
			// 
			// groupBox2
			// 
			this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox2.Controls.Add(this.writeTextBox);
			this.groupBox2.Controls.Add(this.radioButtonHex);
			this.groupBox2.Controls.Add(this.radioButtonUTF8);
			this.groupBox2.Controls.Add(this.radioButtonASCII);
			this.groupBox2.Controls.Add(this.sendButton);
			this.groupBox2.Location = new System.Drawing.Point(6, 60);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(741, 214);
			this.groupBox2.TabIndex = 1;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Write";
			// 
			// writeTextBox
			// 
			this.writeTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.writeTextBox.Location = new System.Drawing.Point(6, 16);
			this.writeTextBox.Multiline = true;
			this.writeTextBox.Name = "writeTextBox";
			this.writeTextBox.Size = new System.Drawing.Size(729, 164);
			this.writeTextBox.TabIndex = 10;
			this.writeTextBox.TextChanged += new System.EventHandler(this.writeTextBox_TextChanged);
			// 
			// radioButtonHex
			// 
			this.radioButtonHex.Appearance = System.Windows.Forms.Appearance.Button;
			this.radioButtonHex.AutoSize = true;
			this.radioButtonHex.Location = new System.Drawing.Point(109, 186);
			this.radioButtonHex.Name = "radioButtonHex";
			this.radioButtonHex.Size = new System.Drawing.Size(78, 23);
			this.radioButtonHex.TabIndex = 9;
			this.radioButtonHex.Text = "Hexadecimal";
			this.radioButtonHex.UseVisualStyleBackColor = true;
			// 
			// radioButtonUTF8
			// 
			this.radioButtonUTF8.Appearance = System.Windows.Forms.Appearance.Button;
			this.radioButtonUTF8.AutoSize = true;
			this.radioButtonUTF8.Checked = true;
			this.radioButtonUTF8.Location = new System.Drawing.Point(56, 186);
			this.radioButtonUTF8.Name = "radioButtonUTF8";
			this.radioButtonUTF8.Size = new System.Drawing.Size(47, 23);
			this.radioButtonUTF8.TabIndex = 8;
			this.radioButtonUTF8.TabStop = true;
			this.radioButtonUTF8.Text = "UTF-8";
			this.radioButtonUTF8.UseVisualStyleBackColor = true;
			// 
			// radioButtonASCII
			// 
			this.radioButtonASCII.Appearance = System.Windows.Forms.Appearance.Button;
			this.radioButtonASCII.AutoSize = true;
			this.radioButtonASCII.Location = new System.Drawing.Point(6, 186);
			this.radioButtonASCII.Name = "radioButtonASCII";
			this.radioButtonASCII.Size = new System.Drawing.Size(44, 23);
			this.radioButtonASCII.TabIndex = 7;
			this.radioButtonASCII.Text = "ASCII";
			this.radioButtonASCII.UseVisualStyleBackColor = true;
			// 
			// sendButton
			// 
			this.sendButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.sendButton.Enabled = false;
			this.sendButton.Location = new System.Drawing.Point(655, 186);
			this.sendButton.Name = "sendButton";
			this.sendButton.Size = new System.Drawing.Size(80, 23);
			this.sendButton.TabIndex = 1;
			this.sendButton.Text = "Send";
			this.sendButton.UseVisualStyleBackColor = true;
			this.sendButton.Click += new System.EventHandler(this.sendButton_Click);
			// 
			// groupBox3
			// 
			this.groupBox3.Controls.Add(this.radioButtonReadHex);
			this.groupBox3.Controls.Add(this.readTextBox);
			this.groupBox3.Controls.Add(this.radioButtonReadUTF8);
			this.groupBox3.Controls.Add(this.radioButtonReadASCII);
			this.groupBox3.Dock = System.Windows.Forms.DockStyle.Fill;
			this.groupBox3.Location = new System.Drawing.Point(770, 3);
			this.groupBox3.Name = "groupBox3";
			this.groupBox3.Size = new System.Drawing.Size(483, 303);
			this.groupBox3.TabIndex = 2;
			this.groupBox3.TabStop = false;
			this.groupBox3.Text = "Read";
			// 
			// radioButtonReadHex
			// 
			this.radioButtonReadHex.Appearance = System.Windows.Forms.Appearance.Button;
			this.radioButtonReadHex.AutoSize = true;
			this.radioButtonReadHex.Location = new System.Drawing.Point(107, 274);
			this.radioButtonReadHex.Name = "radioButtonReadHex";
			this.radioButtonReadHex.Size = new System.Drawing.Size(78, 23);
			this.radioButtonReadHex.TabIndex = 12;
			this.radioButtonReadHex.Text = "Hexadecimal";
			this.radioButtonReadHex.UseVisualStyleBackColor = true;
			this.radioButtonReadHex.CheckedChanged += new System.EventHandler(this.radioButtonReadHex_CheckedChanged);
			// 
			// readTextBox
			// 
			this.readTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.readTextBox.Location = new System.Drawing.Point(6, 16);
			this.readTextBox.Name = "readTextBox";
			this.readTextBox.ReadOnly = true;
			this.readTextBox.Size = new System.Drawing.Size(471, 252);
			this.readTextBox.TabIndex = 0;
			this.readTextBox.Text = "";
			// 
			// radioButtonReadUTF8
			// 
			this.radioButtonReadUTF8.Appearance = System.Windows.Forms.Appearance.Button;
			this.radioButtonReadUTF8.AutoSize = true;
			this.radioButtonReadUTF8.Checked = true;
			this.radioButtonReadUTF8.Location = new System.Drawing.Point(54, 274);
			this.radioButtonReadUTF8.Name = "radioButtonReadUTF8";
			this.radioButtonReadUTF8.Size = new System.Drawing.Size(47, 23);
			this.radioButtonReadUTF8.TabIndex = 11;
			this.radioButtonReadUTF8.TabStop = true;
			this.radioButtonReadUTF8.Text = "UTF-8";
			this.radioButtonReadUTF8.UseVisualStyleBackColor = true;
			this.radioButtonReadUTF8.CheckedChanged += new System.EventHandler(this.radioButtonReadUTF8_CheckedChanged);
			// 
			// radioButtonReadASCII
			// 
			this.radioButtonReadASCII.Appearance = System.Windows.Forms.Appearance.Button;
			this.radioButtonReadASCII.AutoSize = true;
			this.radioButtonReadASCII.Location = new System.Drawing.Point(4, 274);
			this.radioButtonReadASCII.Name = "radioButtonReadASCII";
			this.radioButtonReadASCII.Size = new System.Drawing.Size(44, 23);
			this.radioButtonReadASCII.TabIndex = 10;
			this.radioButtonReadASCII.Text = "ASCII";
			this.radioButtonReadASCII.UseVisualStyleBackColor = true;
			this.radioButtonReadASCII.CheckedChanged += new System.EventHandler(this.radioButtonReadASCII_CheckedChanged);
			// 
			// tableLayoutPanel1
			// 
			this.tableLayoutPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tableLayoutPanel1.ColumnCount = 2;
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 61.13707F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 38.86293F));
			this.tableLayoutPanel1.Controls.Add(this.groupBox3, 1, 0);
			this.tableLayoutPanel1.Controls.Add(this.tabControl1, 0, 0);
			this.tableLayoutPanel1.Location = new System.Drawing.Point(12, 12);
			this.tableLayoutPanel1.Name = "tableLayoutPanel1";
			this.tableLayoutPanel1.RowCount = 1;
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 309F));
			this.tableLayoutPanel1.Size = new System.Drawing.Size(1256, 309);
			this.tableLayoutPanel1.TabIndex = 3;
			// 
			// tabControl1
			// 
			this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tabControl1.Controls.Add(this.tabPage2);
			this.tabControl1.Controls.Add(this.tabPage1);
			this.tabControl1.Location = new System.Drawing.Point(3, 3);
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(761, 303);
			this.tabControl1.TabIndex = 12;
			// 
			// tabPage2
			// 
			this.tabPage2.Controls.Add(this.groupBox5);
			this.tabPage2.Controls.Add(this.groupBox4);
			this.tabPage2.Location = new System.Drawing.Point(4, 22);
			this.tabPage2.Name = "tabPage2";
			this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
			this.tabPage2.Size = new System.Drawing.Size(753, 277);
			this.tabPage2.TabIndex = 1;
			this.tabPage2.Text = "Rich";
			this.tabPage2.UseVisualStyleBackColor = true;
			// 
			// groupBox5
			// 
			this.groupBox5.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox5.Controls.Add(this.label6);
			this.groupBox5.Controls.Add(this.textBoxRichDestination);
			this.groupBox5.Controls.Add(this.apoapseMsgContent);
			this.groupBox5.Controls.Add(this.buttonSendApoapseMsg);
			this.groupBox5.Location = new System.Drawing.Point(6, 60);
			this.groupBox5.Name = "groupBox5";
			this.groupBox5.Size = new System.Drawing.Size(741, 214);
			this.groupBox5.TabIndex = 2;
			this.groupBox5.TabStop = false;
			this.groupBox5.Text = "Apoapse message";
			// 
			// apoapseMsgContent
			// 
			this.apoapseMsgContent.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.apoapseMsgContent.Location = new System.Drawing.Point(6, 47);
			this.apoapseMsgContent.Multiline = true;
			this.apoapseMsgContent.Name = "apoapseMsgContent";
			this.apoapseMsgContent.Size = new System.Drawing.Size(729, 133);
			this.apoapseMsgContent.TabIndex = 10;
			// 
			// buttonSendApoapseMsg
			// 
			this.buttonSendApoapseMsg.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonSendApoapseMsg.Enabled = false;
			this.buttonSendApoapseMsg.Location = new System.Drawing.Point(655, 186);
			this.buttonSendApoapseMsg.Name = "buttonSendApoapseMsg";
			this.buttonSendApoapseMsg.Size = new System.Drawing.Size(80, 23);
			this.buttonSendApoapseMsg.TabIndex = 1;
			this.buttonSendApoapseMsg.Text = "Send";
			this.buttonSendApoapseMsg.UseVisualStyleBackColor = true;
			this.buttonSendApoapseMsg.Click += new System.EventHandler(this.buttonSendApoapseMsg_Click);
			// 
			// groupBox4
			// 
			this.groupBox4.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox4.Controls.Add(this.textBoxRichUsername);
			this.groupBox4.Controls.Add(this.label5);
			this.groupBox4.Controls.Add(this.richConnectButton);
			this.groupBox4.Controls.Add(this.portRich);
			this.groupBox4.Controls.Add(this.label3);
			this.groupBox4.Controls.Add(this.ipAddressRich);
			this.groupBox4.Controls.Add(this.label4);
			this.groupBox4.Location = new System.Drawing.Point(6, 6);
			this.groupBox4.Name = "groupBox4";
			this.groupBox4.Size = new System.Drawing.Size(741, 50);
			this.groupBox4.TabIndex = 1;
			this.groupBox4.TabStop = false;
			this.groupBox4.Text = "Connect";
			// 
			// textBoxRichUsername
			// 
			this.textBoxRichUsername.Location = new System.Drawing.Point(335, 15);
			this.textBoxRichUsername.Name = "textBoxRichUsername";
			this.textBoxRichUsername.Size = new System.Drawing.Size(232, 20);
			this.textBoxRichUsername.TabIndex = 6;
			this.textBoxRichUsername.Text = "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08";
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(271, 18);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(58, 13);
			this.label5.TabIndex = 5;
			this.label5.Text = "Username:";
			// 
			// richConnectButton
			// 
			this.richConnectButton.ForeColor = System.Drawing.Color.ForestGreen;
			this.richConnectButton.Location = new System.Drawing.Point(583, 14);
			this.richConnectButton.Name = "richConnectButton";
			this.richConnectButton.Size = new System.Drawing.Size(85, 23);
			this.richConnectButton.TabIndex = 4;
			this.richConnectButton.Text = "Connect";
			this.richConnectButton.UseVisualStyleBackColor = true;
			this.richConnectButton.Click += new System.EventHandler(this.richConnectButton_Click);
			// 
			// portRich
			// 
			this.portRich.AutoCompleteCustomSource.AddRange(new string[] {
            "3000",
            "55056"});
			this.portRich.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
			this.portRich.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.CustomSource;
			this.portRich.Location = new System.Drawing.Point(180, 15);
			this.portRich.Name = "portRich";
			this.portRich.Size = new System.Drawing.Size(78, 20);
			this.portRich.TabIndex = 3;
			this.portRich.Text = "3000";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(145, 18);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(29, 13);
			this.label3.TabIndex = 2;
			this.label3.Text = "Port:";
			// 
			// ipAddressRich
			// 
			this.ipAddressRich.Location = new System.Drawing.Point(32, 16);
			this.ipAddressRich.Name = "ipAddressRich";
			this.ipAddressRich.Size = new System.Drawing.Size(99, 20);
			this.ipAddressRich.TabIndex = 1;
			this.ipAddressRich.Text = "127.0.0.1";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(6, 19);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(20, 13);
			this.label4.TabIndex = 0;
			this.label4.Text = "IP:";
			// 
			// tabPage1
			// 
			this.tabPage1.Controls.Add(this.groupBox2);
			this.tabPage1.Controls.Add(this.groupBox1);
			this.tabPage1.Location = new System.Drawing.Point(4, 22);
			this.tabPage1.Name = "tabPage1";
			this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
			this.tabPage1.Size = new System.Drawing.Size(753, 277);
			this.tabPage1.TabIndex = 0;
			this.tabPage1.Text = "Raw";
			this.tabPage1.UseVisualStyleBackColor = true;
			// 
			// statusStrip1
			// 
			this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.connectedStatus,
            this.buttonDisconnect});
			this.statusStrip1.Location = new System.Drawing.Point(0, 665);
			this.statusStrip1.Name = "statusStrip1";
			this.statusStrip1.Size = new System.Drawing.Size(1283, 22);
			this.statusStrip1.TabIndex = 4;
			this.statusStrip1.Text = "statusStrip1";
			// 
			// connectedStatus
			// 
			this.connectedStatus.ForeColor = System.Drawing.SystemColors.ControlText;
			this.connectedStatus.Name = "connectedStatus";
			this.connectedStatus.Size = new System.Drawing.Size(79, 17);
			this.connectedStatus.Text = "Disconnected";
			// 
			// buttonDisconnect
			// 
			this.buttonDisconnect.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
			this.buttonDisconnect.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.buttonDisconnect.DropDownButtonWidth = 0;
			this.buttonDisconnect.Image = ((System.Drawing.Image)(resources.GetObject("buttonDisconnect.Image")));
			this.buttonDisconnect.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.buttonDisconnect.Name = "buttonDisconnect";
			this.buttonDisconnect.Size = new System.Drawing.Size(21, 20);
			this.buttonDisconnect.Text = "Disconnect";
			this.buttonDisconnect.ButtonClick += new System.EventHandler(this.buttonDisconnect_ButtonClick);
			// 
			// mainList
			// 
			this.mainList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.mainList.AutoArrange = false;
			this.mainList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnTime,
            this.columnDirection,
            this.columnSize,
            this.columnText,
            this.columnHex});
			this.mainList.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.mainList.HideSelection = false;
			this.mainList.Location = new System.Drawing.Point(15, 327);
			this.mainList.MultiSelect = false;
			this.mainList.Name = "mainList";
			this.mainList.Size = new System.Drawing.Size(1253, 335);
			this.mainList.TabIndex = 5;
			this.mainList.UseCompatibleStateImageBehavior = false;
			this.mainList.View = System.Windows.Forms.View.Details;
			this.mainList.SelectedIndexChanged += new System.EventHandler(this.mainList_SelectedIndexChanged);
			// 
			// columnTime
			// 
			this.columnTime.Text = "Time";
			this.columnTime.Width = 90;
			// 
			// columnDirection
			// 
			this.columnDirection.Text = "Direction";
			// 
			// columnSize
			// 
			this.columnSize.Text = "Size";
			// 
			// columnText
			// 
			this.columnText.Text = "UTF-8";
			this.columnText.Width = 438;
			// 
			// columnHex
			// 
			this.columnHex.Text = "Hex";
			this.columnHex.Width = 397;
			// 
			// button1
			// 
			this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.button1.Location = new System.Drawing.Point(1190, 636);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(75, 23);
			this.button1.TabIndex = 6;
			this.button1.Text = "Clear";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// textBoxRichDestination
			// 
			this.textBoxRichDestination.Location = new System.Drawing.Point(31, 21);
			this.textBoxRichDestination.Name = "textBoxRichDestination";
			this.textBoxRichDestination.Size = new System.Drawing.Size(514, 20);
			this.textBoxRichDestination.TabIndex = 11;
			this.textBoxRichDestination.Text = "apoapse.space:";
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(6, 24);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(23, 13);
			this.label6.TabIndex = 7;
			this.label6.Text = "To:";
			// 
			// MainWindow
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(1283, 687);
			this.Controls.Add(this.tableLayoutPanel1);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.mainList);
			this.Controls.Add(this.statusStrip1);
			this.Name = "MainWindow";
			this.Text = "Apoapse Test Client";
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.groupBox2.ResumeLayout(false);
			this.groupBox2.PerformLayout();
			this.groupBox3.ResumeLayout(false);
			this.groupBox3.PerformLayout();
			this.tableLayoutPanel1.ResumeLayout(false);
			this.tabControl1.ResumeLayout(false);
			this.tabPage2.ResumeLayout(false);
			this.groupBox5.ResumeLayout(false);
			this.groupBox5.PerformLayout();
			this.groupBox4.ResumeLayout(false);
			this.groupBox4.PerformLayout();
			this.tabPage1.ResumeLayout(false);
			this.statusStrip1.ResumeLayout(false);
			this.statusStrip1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox ipAddress;
        private System.Windows.Forms.TextBox port;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button connectButton;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Button sendButton;
        private System.Windows.Forms.RichTextBox readTextBox;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel connectedStatus;
        private System.Windows.Forms.ListView mainList;
        private System.Windows.Forms.ColumnHeader columnTime;
        private System.Windows.Forms.ColumnHeader columnText;
        private System.Windows.Forms.ColumnHeader columnHex;
        private System.Windows.Forms.ColumnHeader columnSize;
        private System.Windows.Forms.RadioButton radioButtonUTF8;
        private System.Windows.Forms.RadioButton radioButtonASCII;
        private System.Windows.Forms.RadioButton radioButtonHex;
        private System.Windows.Forms.RadioButton radioButtonReadHex;
        private System.Windows.Forms.RadioButton radioButtonReadUTF8;
        private System.Windows.Forms.RadioButton radioButtonReadASCII;
        private System.Windows.Forms.ToolStripSplitButton buttonDisconnect;
        private System.Windows.Forms.TextBox writeTextBox;
        private System.Windows.Forms.ColumnHeader columnDirection;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.TextBox textBoxRichUsername;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button richConnectButton;
        private System.Windows.Forms.TextBox portRich;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox ipAddressRich;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.TextBox apoapseMsgContent;
        private System.Windows.Forms.Button buttonSendApoapseMsg;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.TextBox textBoxRichDestination;
	}
}

