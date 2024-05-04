import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import accuracy_score
from sklearn.naive_bayes import GaussianNB
from sklearn.ensemble import RandomForestClassifier
import numpy as np

# File paths for the dataset
file_path = 'C:/Users/Gehan/sensor_data.csv'
cleaned_file_path = 'C:/Users/Gehan/cleaned_sensor_data.csv'

# Preprocess the file to remove rows that do not have the expected number of commas (indicating data issues)
expected_commas = 11  # Expected number of commas in a correct data row
with open(file_path, 'r', encoding="utf-8") as file, open(cleaned_file_path, 'w', encoding="utf-8") as outfile:
    for line in file:
        if line.count(',') == expected_commas:
            outfile.write(line)

# Load and display the cleaned dataset
data = pd.read_csv(cleaned_file_path)
print(data.head())  # Display the first few rows to check data loading
print(data.describe())  # Provide summary statistics of the data

# Check for and handle missing values in the dataset
print(data.isnull().sum())  # Output count of missing values per column
data.dropna(inplace=True)  # Remove rows with missing values

# Convert categorical variable 'char' to numerical codes if it's stored as an object
if data['char'].dtype == 'object':
    char_to_int = {char: idx for idx, char in enumerate(data['char'].unique())}
    int_to_char = {idx: char for char, idx in char_to_int.items()}
    data['char'] = data['char'].map(char_to_int)

# Prepare data for model training
X = data.drop('char', axis=1)  # Feature set
y = data['char']  # Target variable
columns_to_scale = [col for col in X.columns if not col.startswith('contact')]  # Identify columns to scale

# Split data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Scale the selected features for models that are sensitive to variable scales
scaler = StandardScaler()
X_train_scaled = X_train.copy()
X_test_scaled = X_test.copy()
X_train_scaled[columns_to_scale] = scaler.fit_transform(X_train[columns_to_scale])
X_test_scaled[columns_to_scale] = scaler.transform(X_test[columns_to_scale])

# Train a logistic regression model on the scaled data
model = LogisticRegression(max_iter=1000)
model.fit(X_train_scaled, y_train)
predictions = model.predict(X_test_scaled)
accuracy = accuracy_score(y_test, predictions)
print("Accuracy of the logistic regression model: ", accuracy)

# Train and evaluate a Naive Bayes classifier
clf = GaussianNB()
clf.fit(X_train_scaled, y_train)
y_pred = clf.predict(X_test_scaled)
print("Accuracy of the Naive Bayes model: ", accuracy_score(y_test, y_pred))

# Train and evaluate a Random Forest classifier on unscaled data
random_forest_model = RandomForestClassifier(n_estimators=100, random_state=42)
random_forest_model.fit(X_train, y_train)
rf_predictions = random_forest_model.predict(X_test)
print(f"Accuracy of the Random Forest model: {accuracy_score(y_test, rf_predictions)}")