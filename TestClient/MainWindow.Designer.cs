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
            this.useApoapseTransportProtocol = new System.Windows.Forms.CheckBox();
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
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.connectedStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.buttonDisconnect = new System.Windows.Forms.ToolStripSplitButton();
            this.mainList = new System.Windows.Forms.ListView();
            this.columnTime = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnDirection = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnSize = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnText = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHex = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.useApoapseTransportProtocol);
            this.groupBox1.Controls.Add(this.connectButton);
            this.groupBox1.Controls.Add(this.port);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.ipAddress);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(1062, 48);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "TCP Connect (direct)";
            // 
            // useApoapseTransportProtocol
            // 
            this.useApoapseTransportProtocol.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.useApoapseTransportProtocol.AutoSize = true;
            this.useApoapseTransportProtocol.Checked = true;
            this.useApoapseTransportProtocol.CheckState = System.Windows.Forms.CheckState.Checked;
            this.useApoapseTransportProtocol.Location = new System.Drawing.Point(881, 18);
            this.useApoapseTransportProtocol.Name = "useApoapseTransportProtocol";
            this.useApoapseTransportProtocol.Size = new System.Drawing.Size(175, 17);
            this.useApoapseTransportProtocol.TabIndex = 11;
            this.useApoapseTransportProtocol.Text = "Use Apoapse transport protocol";
            this.useApoapseTransportProtocol.UseVisualStyleBackColor = true;
            this.useApoapseTransportProtocol.CheckedChanged += new System.EventHandler(this.useApoapseTransportProtocol_CheckedChanged);
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
            this.groupBox2.Location = new System.Drawing.Point(3, 3);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(525, 214);
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
            this.writeTextBox.Size = new System.Drawing.Size(513, 164);
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
            this.sendButton.Location = new System.Drawing.Point(439, 186);
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
            this.groupBox3.Location = new System.Drawing.Point(534, 3);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(525, 214);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Read";
            // 
            // radioButtonReadHex
            // 
            this.radioButtonReadHex.Appearance = System.Windows.Forms.Appearance.Button;
            this.radioButtonReadHex.AutoSize = true;
            this.radioButtonReadHex.Location = new System.Drawing.Point(109, 186);
            this.radioButtonReadHex.Name = "radioButtonReadHex";
            this.radioButtonReadHex.Size = new System.Drawing.Size(78, 23);
            this.radioButtonReadHex.TabIndex = 12;
            this.radioButtonReadHex.Text = "Hexadecimal";
            this.radioButtonReadHex.UseVisualStyleBackColor = true;
            this.radioButtonReadHex.CheckedChanged += new System.EventHandler(this.radioButtonReadHex_CheckedChanged);
            // 
            // readTextBox
            // 
            this.readTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.readTextBox.Location = new System.Drawing.Point(6, 16);
            this.readTextBox.Name = "readTextBox";
            this.readTextBox.ReadOnly = true;
            this.readTextBox.Size = new System.Drawing.Size(513, 164);
            this.readTextBox.TabIndex = 0;
            this.readTextBox.Text = "";
            // 
            // radioButtonReadUTF8
            // 
            this.radioButtonReadUTF8.Appearance = System.Windows.Forms.Appearance.Button;
            this.radioButtonReadUTF8.AutoSize = true;
            this.radioButtonReadUTF8.Checked = true;
            this.radioButtonReadUTF8.Location = new System.Drawing.Point(56, 186);
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
            this.radioButtonReadASCII.Location = new System.Drawing.Point(6, 186);
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
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Controls.Add(this.groupBox2, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.groupBox3, 1, 0);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(12, 66);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 220F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(1062, 220);
            this.tableLayoutPanel1.TabIndex = 3;
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.connectedStatus,
            this.buttonDisconnect});
            this.statusStrip1.Location = new System.Drawing.Point(0, 636);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(1086, 22);
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
            this.mainList.Location = new System.Drawing.Point(15, 292);
            this.mainList.MultiSelect = false;
            this.mainList.Name = "mainList";
            this.mainList.Size = new System.Drawing.Size(1056, 341);
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
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1086, 658);
            this.Controls.Add(this.mainList);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Controls.Add(this.groupBox1);
            this.Name = "MainWindow";
            this.Text = "Apoapse Test Client";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
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
        private System.Windows.Forms.CheckBox useApoapseTransportProtocol;
    }
}

