import cv2
import numpy as np

# Load a pre-trained human detection model (e.g., HOG descriptor)
hog = cv2.HOGDescriptor()
hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())

def detect_humans(frame):
    # Resize frame for faster processing
    frame = cv2.resize(frame, (640, 480))
    
    # Detect humans in the frame
    humans, _ = hog.detectMultiScale(frame, winStride=(4, 4), padding=(8, 8), scale=1.05)
    
    return humans

def draw_stick_figure(frame, bbox):
    # Extract bounding box coordinates
    x, y, w, h = bbox

    # Draw stick figure
    # Head
    cv2.circle(frame, (int(x + w / 2), y), 10, (0, 255, 255), -1)
    # Torso
    cv2.line(frame, (int(x + w / 2), y), (int(x + w / 2), int(y + h / 2)), (0, 255, 255), 2)
    # Legs
    cv2.line(frame, (int(x + w / 2), int(y + h / 2)), (x, y + h), (0, 255, 255), 2)
    cv2.line(frame, (int(x + w / 2), int(y + h / 2)), (x + w, y + h), (0, 255, 255), 2)
    # Arms
    cv2.line(frame, (int(x + w / 2), int(y + h / 4)), (x, int(y + h / 2)), (0, 255, 255), 2)
    cv2.line(frame, (int(x + w / 2), int(y + h / 4)), (x + w, int(y + h / 2)), (0, 255, 255), 2)

def detect_fall(previous_bbox, current_bbox):
    if previous_bbox is None:
        return False

    _, _, prev_w, prev_h = previous_bbox
    _, _, curr_w, curr_h = current_bbox

    # Calculate aspect ratios
    prev_aspect_ratio = prev_h / prev_w
    curr_aspect_ratio = curr_h / curr_w

    # Calculate movement change
    prev_center_y = previous_bbox[1] + prev_h / 2
    curr_center_y = current_bbox[1] + curr_h / 2

    # Criteria for detecting a fall
    if curr_aspect_ratio < prev_aspect_ratio * 0.5 and curr_center_y > prev_center_y + prev_h * 0.5:
        return True

    return False

def main():
    cap = cv2.VideoCapture(0)
    previous_bboxes = []

    while True:
        ret, frame = cap.read()

        if not ret:
            break

        # Detect humans in the frame
        humans = detect_humans(frame)

        # Initialize fall detection flag
        fall_detected = False
        current_bboxes = []

        # Draw bounding boxes and stick figures around detected humans
        for (x, y, w, h) in humans:
            current_bbox = (x, y, w, h)
            current_bboxes.append(current_bbox)

            for previous_bbox in previous_bboxes:
                if detect_fall(previous_bbox, current_bbox):
                    fall_detected = True

            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
            draw_stick_figure(frame, (x, y, w, h))

        previous_bboxes = current_bboxes

        # Display "Fall Detected" if a fall is detected
        if fall_detected:
            cv2.putText(frame, 'Fall Detected', (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2, cv2.LINE_AA)
            cv2.putText(frame, 'FALL', (int(frame.shape[1] / 2) - 50, int(frame.shape[0] / 2)), cv2.FONT_HERSHEY_SIMPLEX, 2, (0, 0, 255), 4, cv2.LINE_AA)

        # Display the frame
        cv2.imshow('Elder Fall Detection', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
