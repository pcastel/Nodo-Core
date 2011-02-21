/*
 * NodoDueManagerAboutBox.java
 */
package nl.lemval.nododue.dialog;

import java.awt.Cursor;
import java.awt.event.ItemEvent;
import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashSet;
import java.util.TreeSet;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JOptionPane;
import nl.lemval.nododue.NodoDueManager;
import nl.lemval.nododue.Options;
import nl.lemval.nododue.cmd.CommandInfo;
import nl.lemval.nododue.cmd.CommandLoader;
import nl.lemval.nododue.cmd.NodoCommand;
import nl.lemval.nododue.cmd.NodoResponse;
import nl.lemval.nododue.util.SerialCommunicator;
import nl.lemval.nododue.util.listeners.HexKeyListener;
import nl.lemval.nododue.util.listeners.OutputEventListener;
import org.jdesktop.application.Action;

/**
 * Selecteert de units waar de commando's naar toe worden gezonden.
 * De opslag blijft de Options classe.
 *
 * @author Michael
 */
public class UnitSelectorBox extends javax.swing.JDialog {

    private boolean singleUnitSelectionMode = false;

    private static final String ALL_NODOS = "Alle";
    /** Gesorteerde lijst van remote units in het tekstveld */
    private TreeSet<String> remoteUnits = new TreeSet<String>(new Comparator<String>()       {

        public int compare(String o1, String o2) {
            try {
                return (int) (Long.parseLong(o1) - Long.parseLong(o2));
            } catch (Exception e) {
                if (o1 == null) {
                    return -1;
                }
                return o1.compareTo(o2);
            }
        }
    });
    /** Locale unit in het tekstveld */
    private String currentUnit = null;
    /** Validator voor de invoer van remote units */
    private HexKeyListener hexKeyListener = new HexKeyListener(1);
    /** Mogelijke selectie van remote units */
    private DefaultComboBoxModel remoteUnitModel = new DefaultComboBoxModel();

    public UnitSelectorBox(java.awt.Frame parent) {
        super(parent);
        initComponents();
        getRootPane().setDefaultButton(closeButton);
        DialogUtil.configureCloseOnEscape(this);
    }

    @Override
    public void setVisible(boolean state) {
        if (state) {
            refreshDialog();
        }
        super.setVisible(state);
    }

    public void setSingleUnitSelectionMode(boolean singleUnit) {
        singleUnitSelectionMode = singleUnit;
    }

    private void refreshDialog() {
        Options options = Options.getInstance();
        int currentUnitNum = options.getNodoUnit();
        String[] ru = options.getRemoteUnits();
        boolean local = options.isUseLocalUnit();
        boolean remote = options.isUseRemoteUnits();

        remoteUnitModel.removeAllElements();
        remoteUnitModel.addElement(ALL_NODOS);
        for (int i = 1; i < 16; i++) {
            if (i != currentUnitNum) {
                remoteUnitModel.addElement(String.valueOf(i));
            }
        }
        currentUnit = String.valueOf(currentUnitNum);
        localUnitLabel.setText(currentUnit);
        if (ru.length > 0) {
            remoteUnitSelection.setSelectedItem(ru[0]);
            remoteUnits.addAll(Arrays.asList(ru));
            updateRemoteUnits();
        }

        if (local && !remote) {
            localButton.getModel().setSelected(true);
        } else {
            if (!local && ru.length == 1) {
                remoteButton.getModel().setSelected(true);
            } else {
                multipleRemoteButton.getModel().setSelected(true);
                if (local) {
                    includeLocal.getModel().setSelected(true);
                }
            }
        }
    }

    @Action
    public void closeOptionBox() {
        Options options = Options.getInstance();
        if (multipleRemoteButton.isSelected()) {
            options.setUseLocalUnit(includeLocal.isSelected());
            options.setUseRemoteUnits(true);
            options.setRemoteUnits(remoteUnits);
        } else if (remoteButton.isSelected()) {
            options.setUseLocalUnit(false);
            options.setUseRemoteUnits(true);
            String su = (String) remoteUnitSelection.getSelectedItem();
            HashSet<String> set = new HashSet<String>();
            if (ALL_NODOS.equals(su)) {
                su = "0";
            }
            set.add(su);
            options.setRemoteUnits(set);
        } else {
            options.setUseLocalUnit(true);
            options.setUseRemoteUnits(false);
            options.setRemoteUnits(remoteUnits);
        }
        dispose();
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        localOrRemoteGroup = new javax.swing.ButtonGroup();
        javax.swing.JLabel titleLabel = new javax.swing.JLabel();
        javax.swing.JLabel sendToLabel = new javax.swing.JLabel();
        localButton = new javax.swing.JRadioButton();
        localUnitLabel = new javax.swing.JTextField();
        scanLocalButton = new javax.swing.JButton();
        remoteButton = new javax.swing.JRadioButton();
        remoteUnitSelection = new javax.swing.JComboBox();
        scanRemoteButton = new javax.swing.JButton();
        multipleRemoteButton = new javax.swing.JRadioButton();
        remoteUnitList = new javax.swing.JTextField();
        includeLocal = new javax.swing.JCheckBox();
        jPanel1 = new javax.swing.JPanel();
        closeButton = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        org.jdesktop.application.ResourceMap resourceMap = org.jdesktop.application.Application.getInstance(nl.lemval.nododue.NodoDueManager.class).getContext().getResourceMap(UnitSelectorBox.class);
        setTitle(resourceMap.getString("title")); // NOI18N
        setModal(true);
        setName("aboutBox"); // NOI18N
        setResizable(false);

        titleLabel.setFont(titleLabel.getFont().deriveFont(titleLabel.getFont().getStyle() | java.awt.Font.BOLD, titleLabel.getFont().getSize()+4));
        titleLabel.setText(resourceMap.getString("title")); // NOI18N
        titleLabel.setName("titleLabel"); // NOI18N

        sendToLabel.setText(resourceMap.getString("sendToLabel.text")); // NOI18N
        sendToLabel.setName("sendToLabel"); // NOI18N

        localOrRemoteGroup.add(localButton);
        localButton.setSelected(true);
        localButton.setText(resourceMap.getString("localButton.text")); // NOI18N
        localButton.setName("localButton"); // NOI18N

        localUnitLabel.setBackground(resourceMap.getColor("localUnitLabel.background")); // NOI18N
        localUnitLabel.setEditable(false);
        localUnitLabel.setHorizontalAlignment(javax.swing.JTextField.CENTER);
        localUnitLabel.setText(resourceMap.getString("localUnitLabel.text")); // NOI18N
        localUnitLabel.setFocusable(false);
        localUnitLabel.setName("localUnitLabel"); // NOI18N

        javax.swing.ActionMap actionMap = org.jdesktop.application.Application.getInstance(nl.lemval.nododue.NodoDueManager.class).getContext().getActionMap(UnitSelectorBox.class, this);
        scanLocalButton.setAction(actionMap.get("scanLocalUnit")); // NOI18N
        scanLocalButton.setText(resourceMap.getString("scanLocalButton.text")); // NOI18N
        scanLocalButton.setName("scanLocalButton"); // NOI18N

        localOrRemoteGroup.add(remoteButton);
        remoteButton.setText(resourceMap.getString("remoteButton.text")); // NOI18N
        remoteButton.setName("remoteButton"); // NOI18N

        remoteUnitSelection.setModel(remoteUnitModel);
        remoteUnitSelection.setName("remoteUnitSelection"); // NOI18N
        remoteUnitSelection.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                remoteUnitSelectionItemStateChanged(evt);
            }
        });

        scanRemoteButton.setAction(actionMap.get("validateRemoteUnit")); // NOI18N
        scanRemoteButton.setText(resourceMap.getString("scanRemoteButton.text")); // NOI18N
        scanRemoteButton.setName("scanRemoteButton"); // NOI18N

        localOrRemoteGroup.add(multipleRemoteButton);
        multipleRemoteButton.setText(resourceMap.getString("multipleRemoteButton.text")); // NOI18N
        multipleRemoteButton.setName("multipleRemoteButton"); // NOI18N

        remoteUnitList.setText(resourceMap.getString("remoteUnitList.text")); // NOI18N
        remoteUnitList.setName("remoteUnitList"); // NOI18N
        remoteUnitList.addFocusListener(new java.awt.event.FocusAdapter() {
            public void focusLost(java.awt.event.FocusEvent evt) {
                remoteUnitListFocusLost(evt);
            }
        });
        remoteUnitList.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyTyped(java.awt.event.KeyEvent evt) {
                updateRemoteUnits(evt);
            }
        });

        includeLocal.setText(resourceMap.getString("includeLocal.text")); // NOI18N
        includeLocal.setName("includeLocal"); // NOI18N

        jPanel1.setName("jPanel1"); // NOI18N

        closeButton.setAction(actionMap.get("closeOptionBox")); // NOI18N
        closeButton.setText(resourceMap.getString("closeButton.text")); // NOI18N
        closeButton.setName("closeButton"); // NOI18N
        jPanel1.add(closeButton);

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, layout.createSequentialGroup()
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING)
                    .add(layout.createSequentialGroup()
                        .add(12, 12, 12)
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(multipleRemoteButton)
                            .add(remoteButton)
                            .add(localButton))
                        .add(18, 18, 18)
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(includeLocal)
                            .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING)
                                .add(remoteUnitList, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 132, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                                .add(org.jdesktop.layout.GroupLayout.LEADING, layout.createSequentialGroup()
                                    .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING, false)
                                        .add(localUnitLabel)
                                        .add(remoteUnitSelection, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 58, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                                    .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                                    .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING, false)
                                        .add(scanLocalButton, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                        .add(scanRemoteButton)))))
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED, 12, Short.MAX_VALUE))
                    .add(layout.createSequentialGroup()
                        .addContainerGap()
                        .add(titleLabel, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 282, Short.MAX_VALUE))
                    .add(org.jdesktop.layout.GroupLayout.LEADING, layout.createSequentialGroup()
                        .addContainerGap()
                        .add(sendToLabel))
                    .add(jPanel1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 294, Short.MAX_VALUE))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(layout.createSequentialGroup()
                .addContainerGap(org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .add(titleLabel)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(sendToLabel)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(localButton)
                    .add(localUnitLabel, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                    .add(scanLocalButton))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(remoteButton)
                    .add(remoteUnitSelection, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                    .add(scanRemoteButton))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(multipleRemoteButton)
                    .add(remoteUnitList, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(includeLocal)
                .add(12, 12, 12)
                .add(jPanel1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void updateRemoteUnits(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_updateRemoteUnits
        char keychar = evt.getKeyChar();
        if (keychar == KeyEvent.VK_DELETE
                || keychar == KeyEvent.VK_BACK_SPACE
                || remoteUnitList.getSelectedText() != null) {
            remoteUnits.clear();
        }
        if (keychar == KeyEvent.VK_ENTER) {
            updateRemoteUnits();
        }
        boolean active = (remoteUnitList.getText().length() > 0);
        multipleRemoteButton.getModel().setSelected(active);
    }//GEN-LAST:event_updateRemoteUnits

    private void remoteUnitListFocusLost(java.awt.event.FocusEvent evt) {//GEN-FIRST:event_remoteUnitListFocusLost
        updateRemoteUnits();
    }//GEN-LAST:event_remoteUnitListFocusLost

    private void remoteUnitSelectionItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_remoteUnitSelectionItemStateChanged
        if (evt.getStateChange() == ItemEvent.SELECTED) {
            String value = (String) evt.getItem();
            if (ALL_NODOS.equals(value)) {
                scanRemoteButton.setEnabled(false);
            } else {
                scanRemoteButton.setEnabled(true);
            }
            remoteButton.setSelected(true);
        }
    }//GEN-LAST:event_remoteUnitSelectionItemStateChanged

    private void updateRemoteUnits() {
        // Read all elements from the textbox and add those to the list
        String all = remoteUnitList.getText();
        String[] list = all.split(",");
        for (int i = 0; i < list.length; i++) {
            String value = hexKeyListener.toDecValue(list[i].trim());
            if (value != null) {
                int result = Integer.parseInt(value);
                if (result > 0 && result < 16) {
                    remoteUnits.add(value);
                }
            }
        }
        // Skip the current unit
        if (remoteUnits.contains(currentUnit)) {
            remoteUnits.remove(currentUnit);
            includeLocal.getModel().setSelected(true);
        }

        // Update the list
        StringBuilder b = new StringBuilder();
        for (String value : remoteUnits) {
            b.append(Integer.parseInt(value));
            b.append(",");
        }
        if (b.length() > 1) {
            remoteUnitList.setText(b.substring(0, b.length() - 1));
        } else {
            remoteUnitList.setText(null);
        }
    }

    @Action
    public void validateRemoteUnit() {
        String selectedItem = (String) remoteUnitSelection.getSelectedItem();
        if (ALL_NODOS.equals(selectedItem)) {
            selectedItem = "All";
        }

        if (NodoDueManager.hasConnection()) {
            setCursor(new Cursor(Cursor.WAIT_CURSOR));

            SerialCommunicator comm = NodoDueManager.getApplication().getSerialCommunicator();

//            final StringBuilder result = new StringBuilder();
            final ArrayList<NodoResponse> responses = new ArrayList<NodoResponse>();
            OutputEventListener listener = new OutputEventListener()      {

                public void handleOutputLine(String message) {
//                    result.append(message);
                }

                public void handleClear() {
                }

                public void handleNodoResponses(NodoResponse[] data) {
                    for (NodoResponse nodoResponse : data) {
                        responses.add(nodoResponse);
                    }
                }
            };
            comm.addOutputListener(listener);
            CommandInfo info = CommandLoader.get(CommandInfo.Name.Unit);
            int unit = Integer.parseInt(selectedItem);
            NodoCommand cmd = NodoCommand.getStatusCommand(info, unit);
            long t = System.currentTimeMillis();
            boolean found = false;
            HashSet<String> data = new HashSet<String>();
            comm.send(cmd);
            do {
                comm.waitCommand(100);
                // Parse result
                for (NodoResponse nodoResponse : responses) {
//                    System.out.println("Received: " + nodoResponse);
                    if (nodoResponse.is(CommandInfo.Name.Unit)
                            && nodoResponse.is(NodoResponse.Direction.Input)) {
                        data.add(nodoResponse.getCommand().getData1());
                        found = true;
                    }
                }
            } while (!found && (System.currentTimeMillis() - t) < 5000);
            comm.removeOutputListener(listener);
            if (data.size() > 0) {
                Options.getInstance().setRemoteUnits(data);
            } else {
                JOptionPane.showMessageDialog(this, "Remote unit '" + unit + "' not found within 5 seconds");
            }
            setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
        }
    }

    @Action
    public void scanLocalUnit() {
        if (NodoDueManager.hasConnection()) {
            setCursor(new Cursor(Cursor.WAIT_CURSOR));

            SerialCommunicator comm = NodoDueManager.getApplication().getSerialCommunicator();
            OutputEventListener listener = new OutputEventListener()      {

                public void handleOutputLine(String message) {
                }

                public void handleClear() {
                }

                public void handleNodoResponses(NodoResponse[] responses) {
                    Options.getInstance().scanUnitFromResponse(responses);
                }
            };
            comm.addOutputListener(listener);

            CommandInfo info = CommandLoader.get(CommandInfo.Name.Unit);
            NodoCommand cmd = NodoCommand.getStatusCommand(info, 0);
            comm.send(cmd);
            comm.waitCommand(500, 1000);

            comm.removeOutputListener(listener);
            refreshDialog();
            invalidate();
            setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
        }
    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton closeButton;
    private javax.swing.JCheckBox includeLocal;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JRadioButton localButton;
    private javax.swing.ButtonGroup localOrRemoteGroup;
    private javax.swing.JTextField localUnitLabel;
    private javax.swing.JRadioButton multipleRemoteButton;
    private javax.swing.JRadioButton remoteButton;
    private javax.swing.JTextField remoteUnitList;
    private javax.swing.JComboBox remoteUnitSelection;
    private javax.swing.JButton scanLocalButton;
    private javax.swing.JButton scanRemoteButton;
    // End of variables declaration//GEN-END:variables
}
