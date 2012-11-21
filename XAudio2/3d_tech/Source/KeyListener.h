#ifndef KeyListener_h__
#define KeyListener_h__


class KeyListener
{
private:
	bool isPushed;
	bool isDown;
	bool isReleased;
	char key;
protected:
public:
	KeyListener(char pKey);
	~KeyListener();
	void update();
	bool getIsPushed(){
		return isPushed;
	}
	bool getIsDown(){
		return isDown;
	}
	bool getIsReleased(){
		return isReleased;
	}
	char getChar(){
		return key;
	}
};

#endif // KeyListener_h__