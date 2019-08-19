### Drowsiness detection system

# import the necessary packages
from scipy.spatial import distance as dist
from imutils.video import VideoStream
from imutils import face_utils
from threading import Thread
import numpy as np
import playsound
import argparse
import imutils
import time
import dlib
import cv2
import pickle
import smtplib
import random


face_cascade = cv2.CascadeClassifier('cascades/data/haarcascade_frontalface_alt2.xml')
eye_cascade = cv2.CascadeClassifier('cascades/data/haarcascade_eye.xml')
smile_cascade = cv2.CascadeClassifier('cascades/data/haarcascade_smile.xml')

recognizer = cv2.face.LBPHFaceRecognizer_create()
recognizer.read("./recognizers/face-trainner.yml")

labels = {"person_name": 1}
with open("pickles/face-labels.pickle", 'rb') as f:
	og_labels = pickle.load(f)
	labels = {v:k for k,v in og_labels.items()}

def emailConfig():
	s=smtplib.SMTP("smtp.gmail.com", 587)
	s.ehlo()
	s.starttls()
	s.login("heisenberg967z@gmail.com", "Avengers12$")
	#Send the mail
	num = random.randint(0000,9999)
	num = str(num)
	msg = "Subject: <Driver update>\n"
	msg = msg + "Driver of car #" + num + " is not fit to drive at the moment." # The /n separates the message from the headers
	s.sendmail("heisenberg967z@gmail.com", "rishabravi967@gmail.com", msg)



def sound_alarm(path):
	# play an alarm sound
	playsound.playsound(path)

def eye_aspect_ratio(eye):
	# compute the euclidean distances between the two sets of
	# vertical eye landmarks (x, y)-coordinates
	A = dist.euclidean(eye[1], eye[5])
	B = dist.euclidean(eye[2], eye[4])

	# compute the euclidean distance between the horizontal
	# eye landmark (x, y)-coordinates
	C = dist.euclidean(eye[0], eye[3])

	# compute the eye aspect ratio
	ear = (A + B) / (2.0 * C)

	# return the eye aspect ratio
	return ear
 
ap = argparse.ArgumentParser()
ap.add_argument("-p", "--shape-predictor", required=True,
	help="path to facial landmark predictor")
facPat = "C:/Users/SESA544759/Downloads/drowsiness-detection/drowsiness-detection/shape_predictor_68_face_landmarks.dat"
ap.add_argument("-a", "--alarm", type=str, default="",
	help="path alarm .WAV file")
ap.add_argument("-w", "--webcam", type=int, default=0,
	help="index of webcam on system")
#args = vars(ap.parse_args(['--shape-predictor','C:/Users/SESA544759/Desktop/FaceDet/shape_predictor_68_face_landmarks.dat'])) 
args = vars(ap.parse_args(['--shape-predictor','C:/Users/SESA544759/Desktop/FaceDet/shape_predictor_68_face_landmarks.dat','--alarm', 'C:/Users/SESA544759/Desktop/FaceDet/alarm.wav']))

EYE_AR_THRESH = 0.3
EYE_AR_CONSEC_FRAMES = 48

COUNTER = 0
Dcount = 0
ALARM_ON = False

# initialize dlib's face detector (HOG-based) and then create
# the facial landmark predictor
print("[INFO] loading facial landmark predictor")
detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor(args["shape_predictor"])

# grab the indexes of the facial landmarks for the left and
# right eye, respectively
(lStart, lEnd) = face_utils.FACIAL_LANDMARKS_IDXS["left_eye"]
(rStart, rEnd) = face_utils.FACIAL_LANDMARKS_IDXS["right_eye"]

# start the video stream thread
print("[INFO] starting video stream thread")
vs = VideoStream(src=args["webcam"]).start()
time.sleep(1.0)


# loop over frames from the video stream
while True:
	# grab the frame from the threaded video file stream, resize
	# it, and convert it to grayscale
	# channels)
	frame = vs.read()
	frame = imutils.resize(frame, width=450)
	gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
	faces = face_cascade.detectMultiScale(gray, scaleFactor=1.5, minNeighbors=5)
	for (x, y, w, h) in faces:
		#print(x,y,w,h)
		roi_gray = gray[y:y+h, x:x+w] #(ycord_start, ycord_end)
		roi_color = frame[y:y+h, x:x+w]

		id_, conf = recognizer.predict(roi_gray)
		if conf>=4 and conf <= 85:
			#print(5: #id_)
			#print(labels[id_])
			font = cv2.FONT_HERSHEY_SIMPLEX
			name = labels[id_]
			color = (255, 255, 255)
			stroke = 2
			cv2.putText(frame, name, (x,y), font, 1, color, stroke, cv2.LINE_AA)

		img_item = "testIn1.png"
		cv2.imwrite(img_item, roi_color)

		color = (255, 0, 0) #BGR 0-255
		stroke = 2
		end_cord_x = x + w
		end_cord_y = y + h
		cv2.rectangle(frame, (x, y), (end_cord_x, end_cord_y), color, stroke)

	# detect faces in the grayscale frame
	rects = detector(gray, 0)

	# loop over the face detections
	for rect in rects:
		# determine the facial landmarks for the face region, then
		# convert the facial landmark (x, y)-coordinates to a NumPy
		# array
		shape = predictor(gray, rect)
		shape = face_utils.shape_to_np(shape)

		# extract the left and right eye coordinates, then use the
		# coordinates to compute the eye aspect ratio for both eyes
		leftEye = shape[lStart:lEnd]
		rightEye = shape[rStart:rEnd]
		leftEAR = eye_aspect_ratio(leftEye)
		rightEAR = eye_aspect_ratio(rightEye)

		# average the eye aspect ratio together for both eyes
		ear = (leftEAR + rightEAR) / 2.0

		# compute the convex hull for the left and right eye, then
		# visualize each of the eyes
		leftEyeHull = cv2.convexHull(leftEye)
		rightEyeHull = cv2.convexHull(rightEye)
		cv2.drawContours(frame, [leftEyeHull], -1, (0, 255, 0), 1)
		cv2.drawContours(frame, [rightEyeHull], -1, (0, 255, 0), 1)

		# check to see if the eye aspect ratio is below the blink
		# threshold, and if so, increment the blink frame counter
		if ear < EYE_AR_THRESH:
			COUNTER += 1

			# if the eyes were closed for a sufficient number of
			# then sound the alarm
			#if COUNTER >= 5:
			#	emailConfig() #send mail if driver is caught drowsy 5 times
			if COUNTER >= EYE_AR_CONSEC_FRAMES:
				# if the alarm is not on, turn it on
				if not ALARM_ON:
					ALARM_ON = True

					# check to see if an alarm file was supplied,
					# and if so, start a thread to have the alarm
					# sound played in the background
					if args["alarm"] != "":
						t = Thread(target=sound_alarm,
							args=(args["alarm"],))
						t.deamon = True
						t.start()
						Dcount += 1

				# draw an alarm on the frame
				cv2.putText(frame, "ALERT - Drowsy!", (10, 30),
					cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
				if Dcount >= 5: #if caught drowsy > 5 times, send email to authorities.
					emailConfig()

		# otherwise, the eye aspect ratio is not below the blink
		# threshold, so reset the counter and alarm
		else:
			COUNTER = 0
			ALARM_ON = False

		# draw the computed eye aspect ratio on the frame to help
		# with debugging and setting the correct eye aspect ratio
		# thresholds and frame counters
		cv2.putText(frame, "EAR: {:.2f}".format(ear), (300, 30),
			cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
 
	# show the frame
	cv2.imshow("Frame", frame)
	key = cv2.waitKey(1) & 0xFF
 
	# if the `q` key was pressed, break from the loop
	if key == ord("q"):
		cv2.imwrite('testSave1.png',frame)
		break

# do a bit of cleanup
cv2.destroyAllWindows()
vs.stop()