// FileSelector.java
//
// ICS 22 / CSE 22 Fall 2012
// Project #1: Perfect Candidate
//
// YOU ARE NOT REQUIRED TO READ OR UNDERSTAND THIS CODE; YOU MAY NOT
// MODIFY IT.

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;


public class FileSelector extends JDialog
{
	private String selectedFile;
	
	private JTextField fileField;
	
	
	public FileSelector(Frame owner, String title, String prompt, boolean fileIsInput)
	{
		super(owner, title, true);
		
		selectedFile = null;

		setSize(400, 200);

		buildUI(prompt, fileIsInput);

		addWindowListener(
			new WindowAdapter()
			{
				public void windowClosing(WindowEvent e)
				{
					selectedFile = null;
					dispose();
				}
			});
	}

	
	public String getSelectedFile()
	{
		return selectedFile;
	}
	
	
	private void buildUI(String prompt, final boolean fileIsInput)
	{
		getContentPane().removeAll();
		
		GridBagLayout contentLayout = new GridBagLayout();
		setLayout(contentLayout);
		
		JLabel promptLabel = new JLabel(prompt);
		add(promptLabel);
		contentLayout.setConstraints(
			promptLabel,
			new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0,
				GridBagConstraints.EAST, GridBagConstraints.NONE,
				new Insets(5, 5, 5, 5), 0, 0));

		fileField = new JTextField();
		add(fileField);
		contentLayout.setConstraints(
			fileField,
			new GridBagConstraints(1, 0, 1, 1, 1.0, 0.0,
				GridBagConstraints.EAST, GridBagConstraints.BOTH,
				new Insets(5, 5, 5, 5), 0, 0));
		fileField.setText("");

		final FileSelector fs = this;
		
		JButton browseButton = new JButton("Browse");
		browseButton.addActionListener(
			new ActionListener()
			{
				public void actionPerformed(ActionEvent e)
				{
					JFileChooser chooser = new JFileChooser();
					chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
					chooser.setMultiSelectionEnabled(false);

					int result = fileIsInput
						? chooser.showOpenDialog(getContentPane())
						: chooser.showSaveDialog(getContentPane());
					
					if (result == JFileChooser.APPROVE_OPTION)
					{
						if (!fileIsInput && chooser.getSelectedFile().exists())
						{
							int overwrite = JOptionPane.showConfirmDialog(fs,
								"That file already exists.  Overwrite its contents?",
								"Overwrite Existing File?",
								JOptionPane.YES_NO_OPTION,
								JOptionPane.QUESTION_MESSAGE);
							
							if (overwrite == JOptionPane.YES_OPTION)
							{
								fileField.setText(chooser.getSelectedFile().getAbsolutePath());
							}
						}
						else
						{
							fileField.setText(chooser.getSelectedFile().getAbsolutePath());
						}
					}
				}
			});
		add(browseButton);
		contentLayout.setConstraints(
			browseButton,
			new GridBagConstraints(2, 0, 1, 1, 0.0, 0.0,
				GridBagConstraints.WEST, GridBagConstraints.NONE,
				new Insets(5, 5, 5, 5), 0, 0));

		JButton okButton = new JButton("OK");
		okButton.addActionListener(
			new ActionListener()
			{
				public void actionPerformed(ActionEvent e)
				{
					selectedFile = fileField.getText().trim();
					
					if (selectedFile.length() > 0)
					{
						dispose();
					}
					else
					{
						JOptionPane.showMessageDialog(fs,
							"No filename selected",
							"No Filename Selected",
							JOptionPane.ERROR_MESSAGE);
					}
				}
			});
		add(okButton);
		contentLayout.setConstraints(
			okButton,
			new GridBagConstraints(1, 1, 1, 1, 0.0, 0.0,
				GridBagConstraints.EAST, GridBagConstraints.NONE,
				new Insets(15, 5, 5, 5), 0, 0));
	
		JButton cancelButton = new JButton("Cancel");
		cancelButton.addActionListener(
			new ActionListener()
			{
				public void actionPerformed(ActionEvent e)
				{
					selectedFile = null;
					dispose();
				}
			});
		add(cancelButton);
		contentLayout.setConstraints(
			cancelButton,
			new GridBagConstraints(2, 1, 1, 1, 0.0, 0.0,
				GridBagConstraints.EAST, GridBagConstraints.NONE,
				new Insets(15, 5, 5, 5), 0, 0));
	}
}
